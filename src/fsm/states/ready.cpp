#include "canzero/canzero.h"
#include "feedthrough_mosfet.h"
#include "fsm/states.h"
#include "precharge_mosfet.h"
#include "sdc_brake.h"

motor_state fsm::states::ready(motor_command cmd,
                               Duration time_since_last_transition) {

  if (motor_command_DISARM45 == cmd || motor_command_ABORT == cmd) {
    return motor_state_DISARMING45;
  }

  if (motor_command_START == cmd) {
    return motor_state_CONTROL;
  }

  
  pwm::disable_trig1();
  PwmControl control;
  control.duty13 = 0.0f;
  control.duty20 = 0.0f;
  control.duty22 = 0.0f;
  control.duty23 = 0.0f;
  control.duty31 = 0.0f;
  control.duty42 = 0.0f;
  pwm::control(control);
  pwm::enable_output();

  if (!sdc_brake::request_close()) {
    canzero_set_command(motor_command_NONE);
  }

  precharge_mosfet::open();
  feedthrough_mosfet::close();

  return motor_state_READY;
}
