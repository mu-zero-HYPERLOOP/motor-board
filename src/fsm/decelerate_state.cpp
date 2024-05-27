#include "canzero/canzero.h"
#include "firmware/motor_board.h"
#include "firmware/pwm.hpp"
#include "fsm/states.h"

constexpr Duration STATE_TIMEOUT = 10_s;

constexpr Velocity STOP_THRESH = 0.01_mps;

motor_state decelerate_state_next(motor_command cmd,
                                  Duration time_since_last_transition) {

  if (motor_command_DISCONNECT == cmd) {
    return motor_state_IDLE;
  }
  
  if (time_since_last_transition > STATE_TIMEOUT) {
    return motor_state_IDLE;
  }
  
  if (fabs(canzero_get_external_velocity()) < static_cast<float>(STOP_THRESH)) {
    return motor_state_READY;
  }

  pwm::enable_trig0_interrupt();
  pwm::enable_trig1_interrupt();
  pwm::enable_output();
  motor_board::set_sdc(true);
  return motor_state_READY;
}
