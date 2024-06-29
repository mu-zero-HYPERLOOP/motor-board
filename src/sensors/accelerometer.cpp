#include "sensors/accelerometer.h"
#include "util/boxcar.h"
#include "canzero/canzero.h"
#include "util/metrics.h"
#include <avr/pgmspace.h>
#include <cassert>
#include <cmath>

static DMAMEM Acceleration max_acceleration = 0_mps2;

constexpr Acceleration G = 9.80665_mps2;

static DMAMEM BoxcarFilter<Acceleration, 10> filter_y(G);
static DMAMEM BoxcarFilter<Acceleration, 10> filter_z(0_mps2);

static DMAMEM Acceleration offset_x = 0_mps2;
static DMAMEM Acceleration offset_y = 0_mps2;
static DMAMEM Acceleration offset_z = 0_mps2;

static void FASTRUN on_value(const Acceleration &x, const Acceleration &y,
                             const Acceleration &z) {

  const Acceleration acceleration = x + offset_x;
  canzero_set_acceleration(static_cast<float>(acceleration));
  canzero_set_error_acceleration_out_of_range(
      (acceleration.abs() > max_acceleration) ? error_flag_ERROR
                                              : error_flag_OK);
}

void FLASHMEM sensors::accelerometer::begin() {
  
  canzero_set_acceleration(0);
  canzero_set_error_acceleration_out_of_range(error_flag_OK);
  canzero_set_error_acceleration_calibration_failed(error_flag_OK);

  canzero_set_acceleration_calibration_offset(0);
  canzero_set_acceleration_calibration_variance(0);

  switch (range) {
  case motor_board::ACCEL_RANGE_05G:
    max_acceleration = G / 2.0;
    break;
  case motor_board::ACCEL_RANGE_1G:
    max_acceleration = G;
    break;
  case motor_board::ACCEL_RANGE_2G:
    max_acceleration = 2 * G;
    break;
  case motor_board::ACCEL_RANGE_4G:
    max_acceleration = 4 * G;
    break;
  }
  assert(motor_board::register_periodic_accelerometer_reading(MEAS_FREQUENCY,
                                                              range, on_value));
}

void PROGMEM sensors::accelerometer::calibrate() {
  Acceleration x_sum = 0_mps2;

  constexpr float MEAN_ESTIMATION_IT = static_cast<float>(MEAS_FREQUENCY);
  for (unsigned int i = 0; i < static_cast<size_t>(MEAN_ESTIMATION_IT); ++i) {
    const auto &[x, _y, _z] = motor_board::sync_read_acceleration();
    x_sum += x;
    canzero_update_continue(canzero_get_time());
    motor_board::delay(1_ms);
  }
  Acceleration x_average = x_sum / (double)MEAN_ESTIMATION_IT;
  Acceleration x_expected = 0_mps2;
  offset_x = x_expected - x_average;
  canzero_set_acceleration_calibration_offset(static_cast<float>(offset_x));

  bool calibration_ok = true;
  if (std::abs(canzero_get_acceleration_calibration_offset()) > 0.1) {
    calibration_ok = false;
  }

  constexpr float VARIANCE_ESTIMATION_IT = static_cast<float>(MEAS_FREQUENCY);
  float x_var = 0;
  for (unsigned int i = 0; i < static_cast<size_t>(VARIANCE_ESTIMATION_IT);
       ++i) {
    const auto &[x, _y, _z] = motor_board::sync_read_acceleration();
    x_var += std::pow(static_cast<float>((x - x_average)), 2);
    canzero_update_continue(canzero_get_time());
    motor_board::delay(1_ms);
  }
  x_var /= VARIANCE_ESTIMATION_IT - 1;
  x_var = std::sqrt(x_var);

  if (std::abs(canzero_get_acceleration_calibration_variance()) > 0.5) {
    calibration_ok = false;
  }
  canzero_set_error_acceleration_calibration_failed(
      calibration_ok ? error_flag_OK : error_flag_ERROR);

  for (size_t i = 0; i < std::max(filter_y.size(), filter_z.size()); ++i) {
    const auto &[x, y, z] = motor_board::sync_read_acceleration();
    on_value(x, y, z);
  }
}

void FASTRUN sensors::accelerometer::update() {}
