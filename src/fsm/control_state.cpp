#include "canzero/canzero.h"
#include "firmware/motor_board.h"
#include "fsm/states.h"
#include "util/timestamp.h"
#include "firmware/pwm.hpp"

constexpr Duration STATE_TIMEOUT = 10_s;

motor_state control_state_next(motor_command cmd,
                               Duration time_since_last_transition) {

  if (motor_command_DISCONNECT == cmd) {
    return motor_state_IDLE;
  }

  if (time_since_last_transition > STATE_TIMEOUT) {
    return motor_state_DECELERATE;
  }

  if (motor_command_DECELERATE == cmd) {
    return motor_state_DECELERATE;
  }

  pwm::enable_trig0_interrupt();
  pwm::enable_trig1_interrupt();
  pwm::enable_output();
  motor_board::set_sdc(true);

  return motor_state_READY;
}
