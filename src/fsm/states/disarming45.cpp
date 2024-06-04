#include "canzero/canzero.h"
#include "feedthrough_mosfet.h"
#include "firmware/pwm.h"
#include "fsm/states.h"
#include "precharge_mosfet.h"
#include "sdc_brake.h"

constexpr Duration MIN_STATE_TIME = 1_s;

motor_state fsm::states::disarming45(motor_command cmd, 
      Duration time_since_last_transition){

  if (time_since_last_transition > MIN_STATE_TIME){
    return motor_state_IDLE;
  }

  pwm::control(PwmControl());
  pwm::enable_output();
  pwm::disable_trig0();
  pwm::disable_trig1();
  sdc_brake::open();
  precharge_mosfet::open();
  feedthrough_mosfet::open();

  return motor_state_DISARMING45;
}

