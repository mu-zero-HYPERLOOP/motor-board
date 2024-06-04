#include "canzero/canzero.h"
#include "feedthrough_mosfet.h"
#include "firmware/motor_board.h"
#include "fsm/states.h"
#include "precharge_mosfet.h"
#include "sdc_brake.h"

motor_state fsm::states::idle(motor_command cmd, Duration time_since_last_transition) {

  if (motor_command_ARM45 == cmd){
    return motor_state_ARMING45;
  }

  sdc_brake::release_brake();
  sdc_brake::open();

  precharge_mosfet::open();
  feedthrough_mosfet::open();
  
  pwm::disable_output();
  pwm::disable_trig0();
  pwm::disable_trig1();

  return motor_state_IDLE;
}
