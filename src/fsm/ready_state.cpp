#include "canzero/canzero.h"
#include "firmware/motor_board.h"
#include "fsm/states.h"
#include "firmware/pwm.hpp"

constexpr Duration STATE_TIMEOUT = 5_min;

motor_state ready_state_next(motor_command cmd,
                            Duration time_since_last_transition) {

  if (motor_command_DISCONNECT == cmd) {
    return motor_state_IDLE;
  }

  if (time_since_last_transition > STATE_TIMEOUT) {
    return motor_state_IDLE;
  }

  if (motor_command_ACCELERATE == cmd) {
    return motor_state_ACCELERATE;
  }

  pwm::disable_trig0_interrupt();
  pwm::disable_trig1_interrupt();
  pwm::disable_output();
  motor_board::set_sdc(true);

  return motor_state_READY;
}
