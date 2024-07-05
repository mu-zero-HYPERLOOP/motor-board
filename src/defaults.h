#pragma once

#include "canzero/canzero.h"

static void can_defaults() {
  canzero_set_sdc_status(sdc_status_OPEN);

  canzero_set_last_node_missed(255);
  canzero_set_assertion_fault(error_flag_OK);
  canzero_set_error_arming_failed(error_flag_OK);
  canzero_set_error_heartbeat_miss(error_flag_OK);
  canzero_set_error_precharge_failed(error_flag_OK);
  canzero_set_error_mcu_temperature_invalid(error_flag_OK);
  canzero_set_error_lim_temperature1_invalid(error_flag_OK);
  canzero_set_error_lim_temperature2_invalid(error_flag_OK);
  canzero_set_error_lim_temperature3_invalid(error_flag_OK);
  canzero_set_error_lim_temperature4_invalid(error_flag_OK);
  canzero_set_error_acceleration_out_of_range(error_flag_OK);
  canzero_set_error_board_temperature1_invalid(error_flag_OK);
  canzero_set_error_board_temperature2_invalid(error_flag_OK);
  canzero_set_error_board_temperature3_invalid(error_flag_OK);
  canzero_set_error_invalid_target_acceleration(error_flag_OK);
  canzero_set_error_acceleration_calibration_failed(error_flag_OK);

  canzero_set_error_level_current_u1(error_level_OK);
  canzero_set_error_level_current_u2(error_level_OK);
  canzero_set_error_level_current_v1(error_level_OK);
  canzero_set_error_level_current_v2(error_level_OK);
  canzero_set_error_level_current_w1(error_level_OK);
  canzero_set_error_level_current_w2(error_level_OK);
  canzero_set_error_level_vdc_voltage(error_level_OK);
  canzero_set_error_level_lim_temperature(error_level_OK);
  canzero_set_error_level_mcu_temperature(error_level_OK);
  canzero_set_error_level_board_temperature(error_level_OK);

  canzero_set_ignore_45v(bool_t_FALSE);
}
