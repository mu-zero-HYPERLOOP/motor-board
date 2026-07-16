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
#include "sdc_brake.h"
#include <cmath>
#include <algorithm>          // Add this
#include <initializer_list>   // Add this (good practice when using {})
#include <Arduino.h>          // Required for micros()

// ============================================================================
// Configuration
// ============================================================================

/** Spatial phase offset between winding 1 and winding 2 [degrees electrical].
 *  90° = half pole-pitch displacement (typical DLIM sandwich).
 *  Adjust if thrust is lower than expected: try 60° or 120°.
 */
static constexpr float PHASE_OFFSET_DEG = 90.0f;

/** Frequency at which V/f ratio reaches VF_MOD_IDX_AT_BASE [Hz].
 *  Lowered to 15.0 Hz so voltage ramps up much faster to combat inductive reactance. */
static constexpr float VF_BASE_FREQ_HZ = 15.0f;

/** Modulation index at base frequency.
 *  Boosted to match your hard ceiling. Trust your ISR current limiter to roll this back
 *  if the current actually spikes past 50A.
 */
static constexpr float VF_MOD_IDX_AT_BASE = 0.85f;

/** Minimum modulation index at f→0 (resistive boost).
 *  m_min = I_target × R / (Vdc × sqrt(2/3))
 *  With I=5A, R=0.9Ω, Vdc=45V: m_min ≈ 0.12
 */
static constexpr float VF_MOD_IDX_MIN = 0.12f;

/** Hard ceiling on modulation index. */
static constexpr float MOD_IDX_MAX = 0.85f;

/** Soft current limit [A peak]. */
static constexpr float CURRENT_LIMIT_A  = 35.0f;
static constexpr float CURRENT_TARGET_A = 25.0f;

/** Linear Motor Geometry and Slip Limits */
static constexpr float POLE_PITCH_M = 0.065f;      // TODO: Replace with your DLIM's actual pole pitch in meters
static constexpr float MAX_SPEED_MPS = 5.0f;     // Speed at which slip smoothly drops to 0.1
static constexpr float MIN_STARTING_FREQ = 1.0f;  // Minimum frequency at standstill to generate initial thrust
static constexpr float MAX_ELEC_FREQ_HZ = 50.0f; // Maximum electrical frequency

/** Speed Estimation State */
static float s_estimated_speed_mps = 0.0f;

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

static constexpr float MATH_PI_F        = 3.14159265358979323846f;
static constexpr float TWO_PI_F         = 2.0f * MATH_PI_F;
static constexpr float PHASE_OFFSET_RAD = PHASE_OFFSET_DEG * (MATH_PI_F / 180.0f);
static constexpr float DEG120_RAD       = TWO_PI_F / 3.0f;

// ============================================================================
// Helpers
// ============================================================================

static inline float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

// ============================================================================
// Public API
// ============================================================================

/**
 * @brief Estimates mechanical speed fusing CAN data (Acceleration + External Velocity)
 */
static float get_estimated_speed() {
    // Read external velocity from CANZero
    const float external_velocity = canzero_get_external_velocity();
    
    // Integrate external velocity
    s_estimated_speed_mps = external_velocity;
    
    return s_estimated_speed_mps;
}

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
    // mod control with max current safety. 
    if (i_max > CURRENT_LIMIT_A && s_mod_idx > 0.001f) {
        const float m_safe = s_mod_idx * (CURRENT_TARGET_A / i_max);
        desired_mod = std::min(desired_mod, m_safe);
    }

    desired_mod = clampf(desired_mod, 0.0f, MOD_IDX_MAX);

    s_freq_hz = target_freq;
    s_mod_idx = desired_mod;

    // ── 4. Advance electrical angle ───────────────────────────────────────────
    const float f_pwm   = static_cast<float>(pwm::frequency());  // ~20000 Hz
    const float d_theta = TWO_PI_F * s_freq_hz / f_pwm;
    s_theta_rad += d_theta;
    while (s_theta_rad >= TWO_PI_F) s_theta_rad -= TWO_PI_F;
    while (s_theta_rad <  0.0f  ) s_theta_rad += TWO_PI_F;

    // ── 5. Three-phase references ─────────────────────────────────────────────
    // Winding 1: angle θ
    // Winding 2: angle θ + spatial offset (90° for half-pole-pitch DLIM)
    const float t1 = s_theta_rad;
    const float t2 = s_theta_rad + PHASE_OFFSET_RAD;
    float u1_ref = 0;
    float v1_ref = 0;
    float w1_ref = 0;
    float u2_ref = 0;
    float v2_ref = 0;
    float w2_ref = 0;

    if (s_freq_hz > 0) { // forward direction
        u1_ref = s_mod_idx * std::sin(t1);
        v1_ref = s_mod_idx * std::sin(t1 - DEG120_RAD);
        w1_ref = s_mod_idx * std::sin(t1 - 2.0f * DEG120_RAD);

        u2_ref = s_mod_idx * std::sin(t2);
        v2_ref = s_mod_idx * std::sin(t2 - DEG120_RAD);
        w2_ref = s_mod_idx * std::sin(t2 - 2.0f * DEG120_RAD);
    }
    else if (s_freq_hz < 0) // Reverse direction
    {
        w1_ref = s_mod_idx * std::sin(t1);
        v1_ref = s_mod_idx * std::sin(t1 - DEG120_RAD);
        u1_ref = s_mod_idx * std::sin(t1 - 2.0f * DEG120_RAD);

        w2_ref = s_mod_idx * std::sin(t2);
        v2_ref = s_mod_idx * std::sin(t2 - DEG120_RAD);
        u2_ref = s_mod_idx * std::sin(t2 - 2.0f * DEG120_RAD);
    }
    
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
    // 1. Estimate current mechanical velocity using CAN variables
    float v_actual = get_estimated_speed();

    // 2. Dynamic Slip mapping: 0.3 at low speeds (high thrust), 0.1 at max speed (efficiency)
    float abs_v = std::abs(v_actual);
    float target_slip = 0.3f - (0.2f * clampf(abs_v / MAX_SPEED_MPS, 0.0f, 1.0f));

    // 3. Convert mechanical speed to target electrical frequency
    float f_mech = abs_v / (2.0f * POLE_PITCH_M);
    float target_f_e = f_mech / (1.0f - target_slip);

    // Preserve direction based on CAN command, and allow stopping
    float cmd_freq = canzero_get_frequency();
    if (std::abs(cmd_freq) < 0.01f) {
        target_f_e = 0.0f; // Stop condition requested by CAN
    } else {
        // Ensure a minimum electrical frequency to generate thrust at standstill
        target_f_e = std::max(target_f_e, MIN_STARTING_FREQ);
        if (cmd_freq < 0.0f) target_f_e = -target_f_e; // Apply reverse direction
    }

    // ── 4. DIRECTIONAL SAFETY INTERLOCK (SDC CHECK) ──────────────────────────
    // We allow starting from a standstill, so the mismatch check only triggers
    // if the pod has established a physical direction that opposes our commanded magnetic field.
    static constexpr float VELOCITY_DEADBAND_MPS = 0.05f; // Ignore noise below 5 cm/s
    bool direction_mismatch = false;

    if (target_f_e > 0.1f && v_actual < -VELOCITY_DEADBAND_MPS) {
        // Commanded FORWARD, but physically rolling BACKWARD
        direction_mismatch = true;
    } 
    else if (target_f_e < -0.1f && v_actual > VELOCITY_DEADBAND_MPS) {
        // Commanded REVERSE, but physically rolling FORWARD
        direction_mismatch = true;
    }

    if (direction_mismatch) {
        sdc_brake::brake_immediatly(); // Call your SDC brake function
        target_f_e = 0.0f;
        // Do not proceed further in this update cycle.
        // Set targets to zero and return.
        s_target_freq_hz = 0.0f;
        s_target_mod_idx = 0.0f;
        canzero_set_control_active(bool_t_FALSE);
        return; 
    }

    // Override pure CAN frequency setpoint with our newly calculated slip-compensated frequency
    s_target_freq_hz = clampf(
        target_f_e,
        -MAX_ELEC_FREQ_HZ,
        MAX_ELEC_FREQ_HZ
    );
    s_target_mod_idx = clampf(
        canzero_get_modulation_index(),
        0.0f,
        MOD_IDX_MAX
    );

    // TELEMETRY BUG FIX: 
    // Do NOT write the actual frequency back to canzero_set_frequency() here.
    // canzero_set_frequency(s_freq_hz);
    // canzero_set_modulation_index(s_mod_idx);

    // control_active flag
    const motor_state st = canzero_get_state();
    const bool active = (st == motor_state_CONTROL) || (st == motor_state_READY);
    canzero_set_control_active(active ? bool_t_TRUE : bool_t_FALSE);
}
