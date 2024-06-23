#include "firmware/motor_board.h"
#include "canzero/canzero.h"
#include "firmware/ain_scheduler.h"
#include "firmware/mux_scheduler.h"
#include "firmware/xbar.h"
#include "sensors/ext_ntcs.h"
#include "sensors/formula/current_sense.h"
#include "sensors/formula/isolated_voltage.h"
#include "sensors/formula/ntc_beta.h"
#include "sensors/formula/ptx.h"
#include "sensors/formula/voltage_divider.h"
#include "sensors/on_board_ntcs.h"
#include "sensors/phase_current.h"
#include "util/boxcar.h"
#include "util/interval.h"
#include "util/metrics.h"
#include <cstdio>
#include <random>
#include <thread>

static std::random_device rd{};
static std::mt19937 gen{rd()};

static Interval isr_interval(1_min);

constexpr size_t MAX_AIN_PERIODIC_JOBS = 10;
constexpr size_t MAX_MUX_PERIODIC_JOBS = 10;

static AinScheduler<MAX_AIN_PERIODIC_JOBS> ain_scheduler;
static MuxScheduler<MAX_MUX_PERIODIC_JOBS> mux_scheduler;

static uint8_t mux_sel;

static BoxcarFilter<Voltage, 3> vdc_voltage(0_V);

static float v1 = 0;
static float w1 = 0;
static float u1 = 0;
static float v2 = 0;
static float w2 = 0;
static float u2 = 0;

void motor_board::begin() {
  xbar::begin();
  isr_interval = Interval(pwm::frequency());
}

Voltage motor_board::sync_read(ain_pin pin) {
  switch (pin) {
  case ain_mux_21: {
    switch (mux_sel) {
    case 1:
    case 2:
    case 0: {
      const Temperature mock = 24_Celcius;
      std::normal_distribution mockDist{static_cast<float>(mock), 1.0f};
      const Resistance r_ntc = sensors::formula::inv_ntc_beta(
          Temperature(mockDist(gen)), sensors::on_board_ntcs::NTC_BETA,
          sensors::on_board_ntcs::NTC_R_REF, sensors::on_board_ntcs::NTC_T_REF);
      const Voltage v = sensors::formula::vout_of_voltage_divider(
          5_V, r_ntc, sensors::on_board_ntcs::R_MEAS);
      std::normal_distribution dist{static_cast<float>(v), 0.001f};
      return Voltage(dist(gen));
    }
    case 3:
      return 0_V;
    case 4:
    case 5:
    case 6:
    case 7: {
      const Temperature mock = 24_Celcius;
      std::normal_distribution mockDist{static_cast<float>(mock), 0.5f};
      const Resistance r = sensors::formula::inv_ptxx(
          Temperature(mockDist(gen)), sensors::ext_ntcs::PT_R0,
          sensors::ext_ntcs::PT_ALPHA);
      const Voltage v = sensors::formula::vout_of_voltage_divider(
          5_V, r, sensors::ext_ntcs::R_MEAS);
      std::normal_distribution dist{static_cast<float>(v), 0.001f};
      return Voltage(dist(gen));
    }
    }
  case ain_vdc_20:
    switch (canzero_get_state()) {
    case motor_state_INIT:
    case motor_state_IDLE:
    case motor_state_ARMING45:
    case motor_state_DISARMING45:
      return sensors::formula::inv_isolated_voltage(0_V);
    case motor_state_PRECHARGE:
    case motor_state_READY:
    case motor_state_CONTROL:
      return sensors::formula::inv_isolated_voltage(45_V);
    }
  }
  case ain_imeas_w2_19: {
    const Voltage v = sensors::formula::inv_current_sense(
        Current(w2), sensors::phase_current::CURRENT_SENSE_GAIN);
    std::normal_distribution dist{static_cast<float>(v), 0.01f};
    return Voltage(dist(gen));
  }
  case ain_imeas_v2_18: {
    const Voltage v = sensors::formula::inv_current_sense(
        Current(v2), sensors::phase_current::CURRENT_SENSE_GAIN);
    std::normal_distribution dist{static_cast<float>(v), 0.01f};
    return Voltage(dist(gen));
  }
  case ain_imeas_u2_17: {
    const Voltage v = sensors::formula::inv_current_sense(
        Current(u2), sensors::phase_current::CURRENT_SENSE_GAIN);
    std::normal_distribution dist{static_cast<float>(v), 0.01f};
    return Voltage(dist(gen));
  }
  case ain_imeas_u1_16: {
    const Voltage v = sensors::formula::inv_current_sense(
        Current(u1), sensors::phase_current::CURRENT_SENSE_GAIN);
    std::normal_distribution dist{static_cast<float>(v), 0.01f};
    return Voltage(dist(gen));
  }
  case ain_imeas_v1_15: {
    const Voltage v = sensors::formula::inv_current_sense(
        Current(v1), sensors::phase_current::CURRENT_SENSE_GAIN);
    std::normal_distribution dist{static_cast<float>(v), 0.01f};
    return Voltage(dist(gen));
  }
  case ain_imeas_w1_14: {
    const Voltage v = sensors::formula::inv_current_sense(
        Current(w1), sensors::phase_current::CURRENT_SENSE_GAIN);
    std::normal_distribution dist{static_cast<float>(v), 0.01f};
    return Voltage(dist(gen));
  }
  }
}

Voltage motor_board::sync_read(mux_pin pin) {
  return mux_scheduler.sync_read(pin);
}

static std::normal_distribution mcuDist{static_cast<float>(24_Celcius), 0.5f};

Temperature motor_board::read_mcu_temperature() {
  return Temperature(mcuDist(gen));
}

void motor_board::mux_select(uint8_t sel) { mux_sel = sel; }

bool motor_board::register_periodic_reading(
    const Time &period, ain_pin pin, void (*on_value)(const Voltage &v)) {
  return ain_scheduler.register_periodic(period, pin, on_value);
}

bool motor_board::register_periodic_reading(
    const Time &period, mux_pin pin, void (*on_value)(const Voltage &v)) {
  return mux_scheduler.register_periodic(period, pin, on_value);
}

static Interval accel_interval(1_Hz);
static motor_board::AccelerometerRange m_range;
static void (*on_accel_value)(const Acceleration&, const Acceleration&, const Acceleration&) = nullptr;

bool motor_board::register_periodic_accelerometer_reading(
    const Frequency &frequency, AccelerometerRange range,
    void (*on_value)(const Acceleration &x, const Acceleration &y,
                     const Acceleration &z)) {
  accel_interval = Interval(frequency);
  m_range = range;
  on_accel_value = on_value;
  return on_value != nullptr;
}


constexpr Acceleration G = 9.80665_mps2;

static constexpr float ACCEL_VARAIANCE = 0.1f;
static std::normal_distribution lateral{0.0f, ACCEL_VARAIANCE};
static std::normal_distribution vertical{static_cast<float>(G), ACCEL_VARAIANCE};

std::tuple<Acceleration, Acceleration, Acceleration>
motor_board::sync_read_acceleration() {

  std::normal_distribution accel{canzero_get_target_acceleration(), ACCEL_VARAIANCE};

  Acceleration fwd = Acceleration(accel(gen));
  Acceleration vert = Acceleration(vertical(gen));
  Acceleration lat = Acceleration(lateral(gen));
  switch (m_range) {
  case ACCEL_RANGE_05G:
    if (fwd >= 0.5 * G) {
      fwd = 0.5 * G;
    } else if (fwd <= -0.5 * G) {
      fwd = -0.5 * G;
    }
    if (vert >= 0.5 * G) {
      vert = 0.5 * G;
    } else if (vert <= -0.5 * G) {
      vert = -0.5 * G;
    }
    if (lat >= 0.5 * G) {
      lat = 0.5 * G;
    } else if (lat <= -0.5 * G) {
      lat = -0.5 * G;
    }
    break;
  case ACCEL_RANGE_1G:
    if (fwd >= G) {
      fwd = G;
    } else if (fwd <= -G) {
      fwd = -G;
    }
    if (vert >= G) {
      vert = G;
    } else if (vert <= -G) {
      vert = -G;
    }
    if (lat >= G) {
      lat = G;
    } else if (lat <= -G) {
      lat = -G;
    }
    break;
  case ACCEL_RANGE_2G:
    if (fwd >= 2 * G) {
      fwd = 2 * G;
    } else if (fwd <= -2 * G) {
      fwd = -2 * G;
    }
    if (vert >= 2 * G) {
      vert = 2 * G;
    } else if (vert <= -2 * G) {
      vert = -2 * G;
    }
    if (lat >= 2 * G) {
      lat = 2 * G;
    } else if (lat <= -2 * G) {
      lat = -2 * G;
    }
    break;
  case ACCEL_RANGE_4G:
    if (fwd >= 4 * G) {
      fwd = 4 * G;
    } else if (fwd <= -4 * G) {
      fwd = -4 * G;
    }
    if (vert >= 4 * G) {
      vert = 4 * G;
    } else if (vert <= -4 * G) {
      vert = -4 * G;
    }
    if (lat >= 4 * G) {
      lat = 4 * G;
    } else if (lat <= -4 * G) {
      lat = -4 * G;
    }
    break;
  }

  return std::make_tuple(fwd, vert, lat);
}

void motor_board::delay(const Duration &delta) {
  std::this_thread::sleep_for(
      std::chrono::duration<uint32_t, std::micro>(delta.as_us()));
}

void motor_board::set_digital(ctrl_pin pin, bool state) {
  // pass
}

motor_board::InterruptLock motor_board::InterruptLock::acquire() {
  return InterruptLock();
}

void motor_board::InterruptLock::release() {}
motor_board::InterruptLock::~InterruptLock() {}

extern void adc_etc_done0_isr(AdcTrigRes);
extern void adc_etc_done1_isr(AdcTrigRes);

using namespace std::chrono;
static high_resolution_clock::time_point sim_last = high_resolution_clock::now();

extern PwmControl __pwm_control;

void motor_board::update() {
  ain_scheduler.update_continue();
  mux_scheduler.update_continue();

  if (accel_interval.next()){
    if (on_accel_value != nullptr){
      const auto& [x,y,z] = sync_read_acceleration();
      on_accel_value(x,y,z);
    }
  }

  if (isr_interval.next()) {
    AdcTrigRes res;
    if (pwm::trig0_is_enabled()) {
      adc_etc_done0_isr(res);
    }
    if (pwm::trig1_is_enabled()) {
    }
  }

  const high_resolution_clock::time_point now = high_resolution_clock::now();
  const high_resolution_clock::duration dt_duration = now - sim_last;
  float dt = duration_cast<duration<float, std::ratio<1>>>(dt_duration).count();
  sim_last = now;

  constexpr float L = 0.07;
  constexpr float R = 1;

  const float duty_u1 = (__pwm_control.duty20 - 0.5) * 2.0;
  const float u1_v = duty_u1 * canzero_get_vdc_voltage();
  u1 = (u1 + dt * u1_v / L) / (1 + dt * R / L);

  const float duty_v2 = (__pwm_control.duty22 - 0.5) * 2.0;
  const float v2_v = duty_v2 * canzero_get_vdc_voltage();
  v2 = (v2 + dt * v2_v / L) / (1 + dt * R / L);

  const float duty_u2 = (__pwm_control.duty23 - 0.5) * 2.0;
  const float u2_v = duty_u2 * canzero_get_vdc_voltage();
  u2 = (u2 + dt * u2_v / L) / (1 + dt * R / L);

  const float duty_w2 = (__pwm_control.duty42 - 0.5) * 2.0;
  const float w2_v = duty_w2 * canzero_get_vdc_voltage();
  w2 = (w2 + dt * w2_v / L) / (1 + dt * R / L);

  const float duty_w1 = (__pwm_control.duty31 - 0.5) * 2.0;
  const float w1_v = duty_w1 * canzero_get_vdc_voltage();
  w1 = (w1 + dt * w1_v / L) / (1 + dt * R / L);

  const float duty_v1 = (__pwm_control.duty13 - 0.5) * 2.0;
  const float v1_v = duty_v1 * canzero_get_vdc_voltage();
  v1 = (v1 + dt * v1_v / L) / (1 + dt * R / L);
}
