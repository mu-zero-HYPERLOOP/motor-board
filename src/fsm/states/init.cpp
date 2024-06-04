#include "canzero/canzero.h"
#include "feedthrough_mosfet.h"
#include "fsm/states.h"
#include "precharge_mosfet.h"
#include "sdc_brake.h"

motor_state fsm::states::init(motor_command cmd, Duration time_since_last_transition) {
  canzero_set_error_arming_failed(error_flag_OK);
  canzero_set_command(motor_command_NONE);
  canzero_set_error_precharge_failed(error_flag_OK);

  pwm::disable_output();
  pwm::disable_trig0();
  pwm::disable_trig1();

  sdc_brake::open();
  precharge_mosfet::open();
  feedthrough_mosfet::open();

  return motor_state_IDLE;
}
