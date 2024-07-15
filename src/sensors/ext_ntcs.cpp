#include "sensors/ext_ntcs.h"
#include "avr/pgmspace.h"
#include "canzero/canzero.h"
#include "sensors/formula/ntc_beta.h"
#include "error_level_range_check.h"
#include "firmware/motor_board.h"
#include "sensors/formula/ptx.h"
#include "sensors/formula/voltage_divider.h"
#include "util/boxcar.h"
#include <cassert>
#include "print.h"

static DMAMEM BoxcarFilter<Temperature, 100> ext4_filter(24_Celcius);

static DMAMEM ErrorLevelRangeCheck<EXPECT_UNDER>
    error_check(canzero_get_lim_max_temperature,
                canzero_get_error_level_config_lim_temperature,
                canzero_set_error_level_lim_temperature);

static void on_ext4_value(const Voltage &v) {
  using namespace sensors::ext_ntcs;
  if (v < 0.1_V){
    canzero_set_error_lim_temperature4_invalid(error_flag_ERROR);
    canzero_set_lim_temperature4(0);
    return;
  }
  canzero_set_error_lim_temperature4_invalid(error_flag_OK);
  const Resistance r_ntc =
      sensors::formula::r1_of_voltage_divider(5_V, v, R_MEAS);
  const Temperature temp = sensors::formula::ntc_beta(
      r_ntc, 
      sensors::ext_ntcs::NTC_BETA,
      sensors::ext_ntcs::NTC_R_REF,
      sensors::ext_ntcs::NTC_T_REF);
  ext4_filter.push(temp);
  canzero_set_lim_temperature4(
      static_cast<float>(ext4_filter.get() - 0_Celcius));
}

void sensors::ext_ntcs::begin() {

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, mux_pin::mux_ntc_ext_4_7, on_ext4_value));

  canzero_set_lim_temperature1(0);
  canzero_set_lim_temperature2(0);
  canzero_set_lim_temperature3(0);
  canzero_set_lim_temperature4(0);
  canzero_set_lim_average_temperature(0);
  canzero_set_lim_min_temperature(0);
  canzero_set_lim_max_temperature(0);
  canzero_set_error_level_lim_temperature(error_level_OK);
  canzero_set_error_level_config_lim_temperature(error_level_config{
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
}

void sensors::ext_ntcs::calibrate() {
  for (size_t i = 0; i < ext4_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(mux_pin::mux_ntc_ext_4_7);
    on_ext4_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }
  const bool ext4_sensible = ext4_filter.get() > 0_Celcius && ext4_filter.get() <= 1000_Celcius;
  canzero_set_error_lim_temperature4_invalid(ext4_sensible ? error_flag_OK : error_flag_ERROR);


}
void sensors::ext_ntcs::update() {
  canzero_set_lim_average_temperature(canzero_get_lim_temperature4());
  canzero_set_lim_min_temperature(canzero_get_lim_temperature4());
  canzero_set_lim_max_temperature(canzero_get_lim_temperature4());

  error_check.check();
}
