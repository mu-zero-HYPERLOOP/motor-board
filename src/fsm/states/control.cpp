#include "canzero/canzero.h"
#include "feedthrough_mosfet.h"
#include "firmware/motor_board.h"
#include "fsm/states.h"
#include "precharge_mosfet.h"
#include "sdc_brake.h"

motor_state fsm::states::control(motor_command cmd, Duration time_since_last_transition){
  
  if (motor_command_DISARM45 == cmd){
    return motor_state_DISARMING45;
  }

  if (motor_command_STOP == cmd || motor_command_ABORT == cmd){
    return motor_state_READY;
  }

  pwm::enable_output();
  // control is set by ISR!
  pwm::enable_trig0();
  pwm::enable_trig1();

  if (!sdc_brake::request_close()){
    canzero_set_command(motor_command_NONE);
    return motor_state_DISARMING45;
  }

  precharge_mosfet::open();
  feedthrough_mosfet::close();

  return motor_state_CONTROL;
}
