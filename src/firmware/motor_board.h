#pragma once

#include "firmware/adc_etc.hpp"
#include "firmware/pinout.h"
#include "firmware/pwm.hpp"
#include "util/metrics.h"
#include "util/timestamp.h"
#include <cstdint>
#include <tuple>
namespace motor_board {

struct BeginInfo {
  PwmBeginInfo pwmBeginInfo;
  AdcEtcBeginInfo adcBeginInfo;
};

void begin(const BeginInfo& beginInfo);

Voltage sync_read(ain_pin pin);

Voltage sync_read(mux_pin pin);

Temperature read_mcu_temperature();

void set_sdc(bool close);

void mux_select(uint8_t sel);

bool register_periodic_reading(const Time &period, ain_pin pin,
                               void (*on_value)(const Voltage &v));

inline bool register_periodic_reading(const Frequency &frequency, ain_pin pin,
                                      void (*on_value)(const Voltage &v)) {
  return register_periodic_reading(1.0f / frequency, pin, on_value);
}

bool register_periodic_reading(const Time &period, mux_pin pin,
                               void (*on_value)(const Voltage &v));

inline bool register_periodic_reading(const Frequency &frequency, mux_pin pin,
                                      void (*on_value)(const Voltage &v)) {
  return register_periodic_reading(1.0f / frequency, pin, on_value);
}

enum AccelerometerRange {
  ACCEL_RANGE_05G,
  ACCEL_RANGE_1G,
  ACCEL_RANGE_2G,
  ACCEL_RANGE_4G,
};

bool register_periodic_accelerometer_reading(
    const Frequency &frequency, AccelerometerRange range,
    void (*on_value)(const Acceleration &x, const Acceleration &y,
                     const Acceleration &z));

inline bool register_periodic_accelerometer_reading(
    const Time &period, AccelerometerRange range,
    void (*on_value)(const Acceleration &x, const Acceleration &y,
                     const Acceleration &z)) {
  return register_periodic_accelerometer_reading(1.0f / period, range,
                                                 on_value);
}

std::tuple<Acceleration, Acceleration, Acceleration> sync_read_acceleration();

void delay(const Duration& ms);

void set_digital(ctrl_pin pin, bool state);

struct InterruptLock {
public:
  static InterruptLock acquire();
  void release();
  ~InterruptLock();
  InterruptLock(const InterruptLock&) = delete;
  InterruptLock(InterruptLock&&) = delete;
  InterruptLock &operator=(const InterruptLock&) = delete;
  InterruptLock &operator=(InterruptLock&&) = delete;
private:
  InterruptLock() : m_acquired(true) {}
  bool m_acquired;
};

void update_continue();

} // namespace motor_board
