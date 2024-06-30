#include "canzero/canzero.h"
#include "feedthrough_mosfet.h"
#include "fsm/states.h"
#include "precharge_mosfet.h"
#include "sdc_brake.h"

constexpr Duration STATE_TIMEOUT = 5_s;

motor_state fsm::states::arming45(motor_command cmd, Duration time_since_last_transition) {
 
  if (motor_command_DISARM45 == cmd || motor_command_ABORT == cmd){
    return motor_state_DISARMING45;
  }

  if (motor_command_PRECHARGE == cmd){
    return motor_state_PRECHARGE;
  }

  if (time_since_last_transition > STATE_TIMEOUT){
    canzero_set_error_arming_failed(error_flag_ERROR);
    return motor_state_DISARMING45;
  }

  pwm::control(PwmControl());
  pwm::enable_output();
  pwm::disable_trig1();

  if (!sdc_brake::request_close()){
    canzero_set_command(motor_command_NONE);
    return motor_state_DISARMING45;
  }

  precharge_mosfet::open();
  feedthrough_mosfet::open();


  return motor_state_ARMING45;
}
