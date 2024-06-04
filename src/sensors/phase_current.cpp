#include "sensors/phase_current.h"
#include "avr/pgmspace.h"
#include "canzero/canzero.h"
#include "error_level_range_check.h"
#include "firmware/motor_board.h"
#include "firmware/pinout.h"
#include "sensors/formula/current_sense.h"
#include "util/boxcar.h"
#include <cassert>

static DMAMEM BoxcarFilter<Current, 100> v1_filter(0_A);
static DMAMEM BoxcarFilter<Current, 100> w1_filter(0_A);
static DMAMEM BoxcarFilter<Current, 100> u1_filter(0_A);
static DMAMEM BoxcarFilter<Current, 100> v2_filter(0_A);
static DMAMEM BoxcarFilter<Current, 100> w2_filter(0_A);
static DMAMEM BoxcarFilter<Current, 100> u2_filter(0_A);

static DMAMEM ErrorLevelRangeCheck<EXPECT_UNDER>
    v1_current_check(canzero_get_current_v1,
                     canzero_get_error_level_config_current_phase,
                     canzero_set_error_level_current_v1);

static DMAMEM ErrorLevelRangeCheck<EXPECT_UNDER>
    w1_current_check(canzero_get_current_w1,
                     canzero_get_error_level_config_current_phase,
                     canzero_set_error_level_current_w1);

static DMAMEM ErrorLevelRangeCheck<EXPECT_UNDER>
    u1_current_check(canzero_get_current_u1,
                     canzero_get_error_level_config_current_phase,
                     canzero_set_error_level_current_u1);

static DMAMEM ErrorLevelRangeCheck<EXPECT_UNDER>
    v2_current_check(canzero_get_current_v2,
                     canzero_get_error_level_config_current_phase,
                     canzero_set_error_level_current_v2);

static DMAMEM ErrorLevelRangeCheck<EXPECT_UNDER>
    w2_current_check(canzero_get_current_w2,
                     canzero_get_error_level_config_current_phase,
                     canzero_set_error_level_current_w2);

static DMAMEM ErrorLevelRangeCheck<EXPECT_UNDER>
    u2_current_check(canzero_get_current_u2,
                     canzero_get_error_level_config_current_phase,
                     canzero_set_error_level_current_u2);

static void on_v1_value(const Voltage &v) {
  const Current i = sensors::phase_current::convert_v1(v);
  v1_filter.push(i);
  canzero_set_current_v1(static_cast<float>(v1_filter.get()));
}

static void on_w1_value(const Voltage &v) {
  const Current i = sensors::phase_current::convert_w1(v);
  w1_filter.push(i);
  canzero_set_current_w1(static_cast<float>(w1_filter.get()));
}

static void on_u1_value(const Voltage &v) {
  const Current i = sensors::phase_current::convert_u1(v);
  u1_filter.push(i);
  canzero_set_current_u1(static_cast<float>(u1_filter.get()));
}

static void on_v2_value(const Voltage &v) {
  const Current i = sensors::phase_current::convert_v2(v);
  v2_filter.push(i);
  canzero_set_current_v2(static_cast<float>(v2_filter.get()));
}

static void on_w2_value(const Voltage &v) {
  const Current i = sensors::phase_current::convert_w2(v);
  w2_filter.push(i);
  canzero_set_current_w2(static_cast<float>(w2_filter.get()));
}

static void on_u2_value(const Voltage &v) {
  const Current i = sensors::phase_current::convert_u2(v);
  u2_filter.push(i);
  canzero_set_current_u2(static_cast<float>(u2_filter.get()));
}

void sensors::phase_current::begin() {

  canzero_set_current_v1(0);
  canzero_set_current_w1(0);
  canzero_set_current_u1(0);
  canzero_set_current_v2(0);
  canzero_set_current_w2(0);
  canzero_set_current_u2(0);
  canzero_set_current_max(0);
  canzero_set_current_average(0);
  canzero_set_error_level_current_v1(error_level_OK);
  canzero_set_error_level_current_w1(error_level_OK);
  canzero_set_error_level_current_u1(error_level_OK);
  canzero_set_error_level_current_v2(error_level_OK);
  canzero_set_error_level_current_w2(error_level_OK);
  canzero_set_error_level_current_u2(error_level_OK);
  canzero_set_error_level_config_current_phase(error_level_config{
      .m_info_thresh = 10,
      .m_info_timeout = 0.1,
      .m_warning_thresh = 20,
      .m_warning_timeout = 0.1,
      .m_error_thresh = 80,
      .m_error_timeout = 0.1,
      .m_ignore_info = bool_t_FALSE,
      .m_ignore_warning = bool_t_FALSE,
      .m_ignore_error = bool_t_FALSE,
  });

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, ain_pin::ain_imeas_v1_15, on_v1_value));

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, ain_pin::ain_imeas_w1_14, on_w1_value));

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, ain_pin::ain_imeas_u1_16, on_u1_value));

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, ain_pin::ain_imeas_v2_18, on_v2_value));

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, ain_pin::ain_imeas_w2_19, on_w2_value));

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, ain_pin::ain_imeas_u2_17, on_u2_value));
}

void sensors::phase_current::calibrate() {
  for (size_t i = 0; i < v1_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(ain_imeas_v1_15);
    on_v1_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }

  for (size_t i = 0; i < w1_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(ain_imeas_w1_14);
    on_w1_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }

  for (size_t i = 0; i < u1_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(ain_imeas_u1_16);
    on_u1_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }

  for (size_t i = 0; i < v2_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(ain_imeas_v2_18);
    on_v2_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }

  for (size_t i = 0; i < w2_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(ain_imeas_w2_19);
    on_w2_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }

  for (size_t i = 0; i < u2_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(ain_imeas_u2_17);
    on_u2_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }
}

Current sensors::phase_current::convert_v1(Voltage v) {
  return sensors::formula::current_sense(v, CURRENT_SENSE_GAIN, 0.2_mOhm);
}
Current sensors::phase_current::convert_w1(Voltage v) {
  return sensors::formula::current_sense(v, CURRENT_SENSE_GAIN, 0.2_mOhm);
}
Current sensors::phase_current::convert_u1(Voltage v) {
  return sensors::formula::current_sense(v, CURRENT_SENSE_GAIN, 0.2_mOhm);
}
Current sensors::phase_current::convert_v2(Voltage v) {
  return sensors::formula::current_sense(v, CURRENT_SENSE_GAIN, 0.2_mOhm);
}
Current sensors::phase_current::convert_w2(Voltage v) {
  return sensors::formula::current_sense(v, CURRENT_SENSE_GAIN, 0.2_mOhm);
}
Current sensors::phase_current::convert_u2(Voltage v) {
  return sensors::formula::current_sense(v, CURRENT_SENSE_GAIN, 0.2_mOhm);
}

void sensors::phase_current::update() {
  float umax = std::max(canzero_get_current_u1(), canzero_get_current_u2());
  float vmax = std::max(canzero_get_current_v1(), canzero_get_current_v2());
  float wmax = std::max(canzero_get_current_w1(), canzero_get_current_w2());

  float uavg = (canzero_get_current_u1() + canzero_get_current_u2()) / 2.0f;
  float vavg = (canzero_get_current_v1() + canzero_get_current_v2()) / 2.0f;
  float wavg = (canzero_get_current_w1() + canzero_get_current_w2()) / 2.0f;

  canzero_set_current_max(std::max(umax, std::max(vmax, wmax)));
  canzero_set_current_average((uavg + vavg + wavg) / 3.0f);

  v1_current_check.check();
  w1_current_check.check();
  u1_current_check.check();
  v2_current_check.check();
  w2_current_check.check();
  u2_current_check.check();
}
