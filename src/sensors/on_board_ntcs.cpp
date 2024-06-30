#include "sensors/on_board_ntcs.h"
#include "avr/pgmspace.h"
#include "canzero/canzero.h"
#include "error_level_range_check.h"
#include "firmware/motor_board.h"
#include "firmware/pinout.h"
#include "sensors/formula/ntc_beta.h"
#include "sensors/formula/voltage_divider.h"
#include "util/boxcar.h"
#include <algorithm>
#include <cassert>
#include "print.h"

static DMAMEM BoxcarFilter<Temperature, 10> vdc_filter(24_Celcius);
static DMAMEM BoxcarFilter<Temperature, 10> v1_filter(24_Celcius);
static DMAMEM BoxcarFilter<Temperature, 10> v2_filter(24_Celcius);

static DMAMEM ErrorLevelRangeCheck<EXPECT_UNDER>
    error_check(canzero_get_board_max_temperature,
                canzero_get_error_level_config_board_temperature,
                canzero_set_error_level_board_temperature);

static void on_ntc_vdc_connector_value(const Voltage &v) {
  using namespace sensors::on_board_ntcs;
  const Resistance r_ntc =
      sensors::formula::r1_of_voltage_divider(5_V, v, R_MEAS);
  const Temperature temp =
      sensors::formula::ntc_beta(r_ntc, NTC_BETA, NTC_R_REF, NTC_T_REF);
  vdc_filter.push(temp);
  canzero_set_board_temperature1(
      static_cast<float>(vdc_filter.get() - 0_Celcius));
}

static void on_ntc_phase1_value(const Voltage &v) {
  using namespace sensors::on_board_ntcs;
  const Resistance r_ntc =
      sensors::formula::r1_of_voltage_divider(5_V, v, R_MEAS);
  const Temperature temp =
      sensors::formula::ntc_beta(r_ntc, NTC_BETA, NTC_R_REF, NTC_T_REF);
  v1_filter.push(temp);
  canzero_set_board_temperature2(
      static_cast<float>(v1_filter.get() - 0_Celcius));
}

static void on_ntc_phase2_value(const Voltage &v) {
  using namespace sensors::on_board_ntcs;
  const Resistance r_ntc =
      sensors::formula::r1_of_voltage_divider(5_V, v, R_MEAS);
  const Temperature temp =
      sensors::formula::ntc_beta(r_ntc, NTC_BETA, NTC_R_REF, NTC_T_REF);
  v2_filter.push(temp);
  canzero_set_board_temperature3(
      static_cast<float>(v2_filter.get() - 0_Celcius));
}

void sensors::on_board_ntcs::begin() {
  canzero_set_board_temperature1(0);
  canzero_set_board_temperature2(0);
  canzero_set_board_temperature3(0);
  canzero_set_board_avg_temperature(0);
  canzero_set_board_min_temperature(0);
  canzero_set_board_max_temperature(0);
  canzero_set_error_level_board_temperature(error_level_OK);
  canzero_set_error_level_config_board_temperature(error_level_config{
      .m_info_thresh = 45,
      .m_info_timeout = 5,
      .m_warning_thresh = 65,
      .m_warning_timeout = 5,
      .m_error_thresh = 80,
      .m_error_timeout = 5,
      .m_ignore_info = bool_t_FALSE,
      .m_ignore_warning = bool_t_FALSE,
      .m_ignore_error = bool_t_FALSE,
  });

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, mux_pin::mux_ntc_3_0, on_ntc_vdc_connector_value));

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, mux_pin::mux_ntc_2_1, on_ntc_phase1_value));

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, mux_pin::mux_ntc_1_2, on_ntc_phase2_value));
}
void sensors::on_board_ntcs::calibrate() {

  for (size_t i = 0; i < vdc_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(mux_pin::mux_ntc_3_0);
    on_ntc_vdc_connector_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }

  for (size_t i = 0; i < v1_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(mux_pin::mux_ntc_2_1);
    on_ntc_phase1_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }

  for (size_t i = 0; i < v2_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(mux_pin::mux_ntc_1_2);
    on_ntc_phase2_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }
}
void sensors::on_board_ntcs::update() {

  const Temperature avg =
      (vdc_filter.get() + v1_filter.get() + v2_filter.get()) / 3.0;

  const Temperature min =
      Temperature(std::min(std::min(static_cast<float>(vdc_filter.get()),
                                    static_cast<float>(v1_filter.get())),
                           static_cast<float>(v2_filter.get())));

  const Temperature max =
      Temperature(std::max(std::max(static_cast<float>(vdc_filter.get()),
                                    static_cast<float>(v1_filter.get())),
                           static_cast<float>(v2_filter.get())));

  canzero_set_board_min_temperature(static_cast<float>(min - 0_Celcius));
  canzero_set_board_max_temperature(static_cast<float>(max - 0_Celcius));
  canzero_set_board_avg_temperature(static_cast<float>(avg - 0_Celcius));

  error_check.check();
}
