#include "fsm/error_handling.h"
#include "canzero/canzero.h"
#include <algorithm>
#include <array>

motor_command fsm::error_handling::approve(motor_command cmd) {

  const auto error_flags = std::array<error_flag, 6>{
      canzero_get_error_arming_failed(),
      canzero_get_error_precharge_failed(),
      canzero_get_error_acceleration_calibration_failed(),
      canzero_get_error_acceleration_out_of_range(),
      /* canzero_get_error_lim_temperature1_invalid(), */
      /* canzero_get_error_lim_temperature2_invalid(), */
      /* canzero_get_error_lim_temperature3_invalid(), */
      /* canzero_get_error_lim_temperature4_invalid(), */
      canzero_get_assertion_fault(),
      canzero_get_error_heartbeat_miss(),
  };

  const auto max_error_flag_it =
      std::max_element(error_flags.begin(), error_flags.end());
  const error_flag max_error_flag = *max_error_flag_it;
  if (max_error_flag == error_flag_ERROR) {
    // early bail out.
    return motor_command_DISARM45;
  }

  const auto error_levels = std::array<error_level, 2>{
      /* canzero_get_error_level_current_u1(), */
      /* canzero_get_error_level_current_u2(), */
      /* canzero_get_error_level_current_v1(), */
      /* canzero_get_error_level_current_v2(), */
      /* canzero_get_error_level_current_w1(), */
      /* canzero_get_error_level_current_w2(), */
      canzero_get_error_level_vdc_voltage(),
      /* canzero_get_error_level_board_temperature(), */
      canzero_get_error_level_mcu_temperature(),
      /* canzero_get_error_level_lim_temperature(), */
  };
  const auto max_error_level_it =
      std::max_element(error_levels.begin(), error_levels.end());
  const error_level max_error_level = *max_error_level_it;

  const error_level error_level = std::max(
      max_error_level,
      (max_error_flag == error_flag_OK) ? error_level_OK : error_level_ERROR);
  switch (error_level) {
  case error_level_OK:
  case error_level_INFO:
    return cmd;
  case error_level_WARNING:
    return motor_command_ABORT;
  case error_level_ERROR:
    return motor_command_DISARM45;
  default:
    return motor_command_DISARM45;
  }
}
