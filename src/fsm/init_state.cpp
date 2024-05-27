#include "canzero/canzero.h"
#include "firmware/motor_board.h"
#include "fsm/states.h"
#include "firmware/pwm.hpp"

motor_state init_state_next(motor_command cmd,
                            Duration time_since_last_transition) {
  // nothing happending here

  pwm::disable_trig0_interrupt();
  pwm::disable_trig1_interrupt();
  pwm::disable_output();
  motor_board::set_digital(ctrl_pin::precharge_done, false);
  motor_board::set_digital(ctrl_pin::precharge_start, false);

  motor_board::set_sdc(false);
  return motor_state_IDLE;
}
