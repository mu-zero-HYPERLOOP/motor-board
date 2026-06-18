/**
 * @file control.cpp
 * @brief Open-loop V/f control for Double-sided Linear Induction Motor (DLIM)
 *
 * Hardware: Two independent 3-phase windings, star (Wye) configuration.
 *           Each phase: R ≈ 0.9 Ω, L ≈ 4–20 mH (4 coils in series).
 *           DC bus: 45 V max.
 *
 * IMPORTANT: HIGH MODULATION INDEX (m > 0.5) PAIRED WITH LOW FREQUENCY (f < 1 Hz)
 * !!!!!!!!!!!!!!!!!     MAY EXCEED CURRENT LIMITS.     !!!!!!!!!!!!!!!!!!!
 * 
 * Key design decision: setpoint variables (s_target_*) are written ONLY
 * by control::update() from the main loop, and read ONLY by control_loop()
 * in the ISR.  The ISR never calls canzero_set_frequency() etc. — those
 * CAN fields are used as INPUTS (setpoints), not outputs.
 * Telemetry is published from the main-loop update() only.
 *
 * ── Impedance reference (L_total = 12 mH midpoint) ──────────────────────
 *  f =  5 Hz  Z ≈ 0.97 Ω  I_peak(m=1.0) ≈ 37.8 A  → need current limit
 *  f = 20 Hz  Z ≈ 1.76 Ω  I_peak(m=1.0) ≈ 20.8 A
 *  f = 50 Hz  Z ≈ 3.87 Ω  I_peak(m=1.0) ≈  9.5 A
 * ─────────────────────────────────────────────────────────────────────────
 */

#include "control.h"
#include "canzero/canzero.h"
#include "firmware/motor_pwm.h"
#include "firmware/pwm.h"
#include "util/metrics.h"
#include <cmath>
#include <algorithm>          // Add this
#include <initializer_list>   // Add this (good practice when using {})

// ============================================================================
// Configuration
// ============================================================================

/** Spatial phase offset between winding 1 and winding 2 [degrees electrical].
 *  90° = half pole-pitch displacement (typical DLIM sandwich).
 *  Adjust if thrust is lower than expected: try 60° or 120°.
 */
static constexpr float PHASE_OFFSET_DEG = 90.0f;

/** Frequency at which V/f ratio reaches VF_MOD_IDX_AT_BASE [Hz]. */
static constexpr float VF_BASE_FREQ_HZ = 1.0f;

/** Modulation index at base frequency.
 *  At 25 Hz, m=0.5 → I_peak ≈ (0.5 × 36.7 V) / 2.12 Ω ≈ 8.6 A  (safe).
 */
static constexpr float VF_MOD_IDX_AT_BASE = 0.5f;

/** Minimum modulation index at f→0 (resistive boost).
 *  m_min = I_target × R / (Vdc × sqrt(2/3))
 *  With I=5A, R=0.9Ω, Vdc=45V: m_min ≈ 0.12
 */
static constexpr float VF_MOD_IDX_MIN = 0.12f;

/** Hard ceiling on modulation index. */
static constexpr float MOD_IDX_MAX = 0.85f;

/** Soft current limit [A peak]. */
static constexpr float CURRENT_LIMIT_A  = 50.0f;
static constexpr float CURRENT_TARGET_A = 30.0f;

// ============================================================================
// Internal state
// ============================================================================

// ISR-private state.
static float s_theta_rad = 0.0f;
static float s_freq_hz   = 0.0f;
static float s_mod_idx   = 0.0f;

// Setpoints: written by update() (main loop), read by control_loop() (ISR).
// Single float read/write is atomic on Cortex-M7 with FPU enabled.
static volatile float s_target_freq_hz = 0.0f;
static volatile float s_target_mod_idx = 0.0f;  // 0 = auto V/f

static constexpr float TWO_PI           = 2.0f * static_cast<float>(M_PI);
static constexpr float PHASE_OFFSET_RAD = PHASE_OFFSET_DEG * (static_cast<float>(M_PI) / 180.0f);
static constexpr float DEG120_RAD       = TWO_PI / 3.0f;

// ============================================================================
// Helpers
// ============================================================================

static inline float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

// ============================================================================
// Public API
// ============================================================================

void control::begin() {
    s_theta_rad      = 0.0f;
    s_freq_hz        = 0.0f;
    s_mod_idx        = 0.0f;
    s_target_freq_hz = 0.0f;
    s_target_mod_idx = 0.0f;
}

/**
 * @brief Core V/f control — called from ADC-ETC ISR every PWM cycle (20 kHz).
 *
 * Reads setpoints from s_target_* (written by main loop).
 * Does NOT call any canzero_set_* — telemetry is in update() only.
 */
MotorPwmControl control::control_loop(Voltage /*vdc*/) {

    // ── 1. Read setpoints (set by main loop, not by this ISR) ────────────────
    const float target_freq = s_target_freq_hz;
    const float target_mod  = s_target_mod_idx;

    // ── 2. V/f modulation index ───────────────────────────────────────────────
    const float abs_freq = std::abs(target_freq);
    const float vf_mod   = VF_MOD_IDX_MIN
                         + (VF_MOD_IDX_AT_BASE - VF_MOD_IDX_MIN)
                           * clampf(abs_freq / VF_BASE_FREQ_HZ, 0.0f, 1.0f);

    // Manual override: if modulation_index was set > 0 in CAN, use it directly.
    float desired_mod = (target_mod > 0.001f) ? target_mod : vf_mod;

    /* ── 3. Current-limiting backstop ─────────────────────────────────────────*/
    float i_max = std::max<float>({
        std::abs(canzero_get_current_u1()),
        std::abs(canzero_get_current_v1()),
        std::abs(canzero_get_current_w1()),
        std::abs(canzero_get_current_u2()),
        std::abs(canzero_get_current_v2()),
        std::abs(canzero_get_current_w2())
    });

    if (i_max > CURRENT_LIMIT_A && s_mod_idx > 0.001f) {
        const float m_safe = s_mod_idx * (CURRENT_TARGET_A / i_max);
        desired_mod = std::min(desired_mod, m_safe);
    }

    desired_mod = clampf(desired_mod, 0.0f, MOD_IDX_MAX);

    s_freq_hz = target_freq;
    s_mod_idx = desired_mod;

    // ── 4. Advance electrical angle ───────────────────────────────────────────
    const float f_pwm   = static_cast<float>(pwm::frequency());  // ~20000 Hz
    const float d_theta = TWO_PI * s_freq_hz / f_pwm;
    s_theta_rad += d_theta;
    while (s_theta_rad >= TWO_PI) s_theta_rad -= TWO_PI;
    while (s_theta_rad <  0.0f  ) s_theta_rad += TWO_PI;

    // ── 5. Three-phase references ─────────────────────────────────────────────
    // Winding 1: angle θ
    // Winding 2: angle θ + spatial offset (90° for half-pole-pitch DLIM)
    const float t1 = s_theta_rad;
    const float t2 = s_theta_rad + PHASE_OFFSET_RAD;
    const float u1_ref = s_mod_idx * std::sin(t1);
    const float v1_ref = s_mod_idx * std::sin(t1 - DEG120_RAD);
    const float w1_ref = s_mod_idx * std::sin(t1 - 2.0f * DEG120_RAD);

    const float u2_ref = s_mod_idx * std::sin(t2);
    const float v2_ref = s_mod_idx * std::sin(t2 - DEG120_RAD);
    const float w2_ref = s_mod_idx * std::sin(t2 - 2.0f * DEG120_RAD);

    // ── 6. Duty cycles  (center-aligned: 0.5 ± m/2) ──────────────────────────
    MotorPwmControl ctrl;
    ctrl.U1_duty = 0.5f + 0.5f * u1_ref;
    ctrl.V1_duty = 0.5f + 0.5f * v1_ref;
    ctrl.W1_duty = 0.5f + 0.5f * w1_ref;
    ctrl.U2_duty = 0.5f + 0.5f * u2_ref;
    ctrl.V2_duty = 0.5f + 0.5f * v2_ref;
    ctrl.W2_duty = 0.5f + 0.5f * w2_ref;

    return ctrl;
}

/**
 * @brief Main-loop task — reads CAN setpoints, publishes telemetry.
 *
 * This is the ONLY place that calls canzero_get_frequency() /
 * canzero_get_modulation_index() and bridges them to the ISR via
 * s_target_* variables.
 *
 * To command the motor from your CAN tool:
 *   Write OD index 26 (frequency)        e.g. 20.0 Hz
 *   Write OD index 27 (modulation_index) leave at 0 for auto V/f
 */
void control::update() {
    // Bridge CAN setpoints → ISR-readable variables.
    s_target_freq_hz = clampf(
        canzero_get_frequency(),
        -VF_BASE_FREQ_HZ * 3.0f,
         VF_BASE_FREQ_HZ * 3.0f
    );
    s_target_mod_idx = clampf(
        canzero_get_modulation_index(),
        0.0f,
        MOD_IDX_MAX
    );

    // Publish actual values as read-back telemetry.
    // Note: s_freq_hz / s_mod_idx are written by ISR; a torn read here
    // is harmless (just slightly stale telemetry).
    canzero_set_frequency(s_freq_hz);
    canzero_set_modulation_index(s_mod_idx);

    // control_active flag
    const motor_state st = canzero_get_state();
    const bool active = (st == motor_state_CONTROL) || (st == motor_state_READY);
    canzero_set_control_active(active ? bool_t_TRUE : bool_t_FALSE);
}
