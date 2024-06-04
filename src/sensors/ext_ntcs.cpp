#include "sensors/ext_ntcs.h"
#include "avr/pgmspace.h"
#include "canzero/canzero.h"
#include "error_level_range_check.h"
#include "firmware/motor_board.h"
#include "sensors/formula/ptx.h"
#include "sensors/formula/voltage_divider.h"
#include "util/boxcar.h"
#include <cassert>
#include "print.h"

static DMAMEM BoxcarFilter<Temperature, 100> ext1_filter(24_Celcius);
static DMAMEM BoxcarFilter<Temperature, 100> ext2_filter(24_Celcius);
static DMAMEM BoxcarFilter<Temperature, 100> ext3_filter(24_Celcius);
static DMAMEM BoxcarFilter<Temperature, 100> ext4_filter(24_Celcius);

static DMAMEM ErrorLevelRangeCheck<EXPECT_UNDER>
    error_check(canzero_get_lim_max_temperature,
                canzero_get_error_level_config_lim_temperature,
                canzero_set_error_level_lim_temperature);

static void on_ext1_value(const Voltage &v) {
  using namespace sensors::ext_ntcs;
  const Resistance r_ntc =
      sensors::formula::r1_of_voltage_divider(5_V, v, R_MEAS);
  const Temperature temp = sensors::formula::ptxxx(r_ntc, PT_R0, PT_ALPHA);
  ext1_filter.push(temp);
  canzero_set_lim_temperature1(
      static_cast<float>(ext1_filter.get() - 0_Celcius));
}

static void on_ext2_value(const Voltage &v) {
  using namespace sensors::ext_ntcs;
  const Resistance r_ntc =
      sensors::formula::r1_of_voltage_divider(5_V, v, R_MEAS);
  const Temperature temp = sensors::formula::ptxxx(r_ntc, PT_R0, PT_ALPHA);
  ext2_filter.push(temp);
  canzero_set_lim_temperature2(
      static_cast<float>(ext2_filter.get() - 0_Celcius));
}

static void on_ext3_value(const Voltage &v) {
  using namespace sensors::ext_ntcs;
  const Resistance r_ntc =
      sensors::formula::r1_of_voltage_divider(5_V, v, R_MEAS);
  const Temperature temp = sensors::formula::ptxxx(r_ntc, PT_R0, PT_ALPHA);
  ext3_filter.push(temp);
  canzero_set_lim_temperature3(
      static_cast<float>(ext3_filter.get() - 0_Celcius));
}

static void on_ext4_value(const Voltage &v) {
  using namespace sensors::ext_ntcs;
  const Resistance r_ntc =
      sensors::formula::r1_of_voltage_divider(5_V, v, R_MEAS);
  const Temperature temp = sensors::formula::ptxxx(r_ntc, PT_R0, PT_ALPHA);
  ext4_filter.push(temp);
  canzero_set_lim_temperature4(
      static_cast<float>(ext4_filter.get() - 0_Celcius));
}

void sensors::ext_ntcs::begin() {

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, mux_pin::mux_ntc_ext_1_4, on_ext1_value));

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, mux_pin::mux_ntc_ext_2_5, on_ext2_value));

  assert(motor_board::register_periodic_reading(
      MEAS_FREQUENCY, mux_pin::mux_ntc_ext_3_6, on_ext3_value));

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
  for (size_t i = 0; i < ext1_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(mux_pin::mux_ntc_ext_1_4);
    on_ext1_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }
  const bool ext1_sensible = ext1_filter.get() > 0_Celcius && ext1_filter.get() <= 1000_Celcius;
  canzero_set_error_lim_temperature1_invalid(ext1_sensible ? error_flag_OK : error_flag_ERROR);

  for (size_t i = 0; i < ext2_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(mux_pin::mux_ntc_ext_2_5);
    on_ext2_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }
  const bool ext2_sensible = ext2_filter.get() > 0_Celcius && ext2_filter.get() <= 1000_Celcius;
  canzero_set_error_lim_temperature2_invalid(ext2_sensible ? error_flag_OK : error_flag_ERROR);

  for (size_t i = 0; i < ext3_filter.size(); ++i) {
    const Voltage v = motor_board::sync_read(mux_pin::mux_ntc_ext_3_6);
    on_ext3_value(v);
    motor_board::delay(1_ms);
    canzero_update_continue(canzero_get_time());
  }
  const bool ext3_sensible = ext3_filter.get() > 0_Celcius && ext3_filter.get() <= 1000_Celcius;
  canzero_set_error_lim_temperature3_invalid(ext3_sensible ? error_flag_OK : error_flag_ERROR);

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
  const Temperature avg = (ext1_filter.get() + ext2_filter.get() +
                           ext3_filter.get() + ext4_filter.get()) /
                          4.0;

  const Temperature min =
      Temperature(std::min(std::min(static_cast<float>(ext1_filter.get()),
                                    static_cast<float>(ext2_filter.get())),
                           std::min(static_cast<float>(ext3_filter.get()),
                                    static_cast<float>(ext4_filter.get()))));

  const Temperature max =
      Temperature(std::max(std::max(static_cast<float>(ext1_filter.get()),
                                    static_cast<float>(ext2_filter.get())),
                           std::max(static_cast<float>(ext3_filter.get()),
                                    static_cast<float>(ext4_filter.get()))));

  canzero_set_lim_average_temperature(static_cast<float>(avg - 0_Celcius));
  canzero_set_lim_min_temperature(static_cast<float>(min - 0_Celcius));
  canzero_set_lim_max_temperature(static_cast<float>(max - 0_Celcius));

  error_check.check();
}
