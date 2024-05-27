#include "canzero/canzero.h"
#include "firmware/motor_board.h"
#include "firmware/pwm.hpp"
#include "fsm/states.h"
#include "util/metrics.h"

constexpr Duration PRECHARGE_TIMETHRESH = 1_s;
constexpr Voltage PRECHARGE_THRESH = 30_V;

constexpr Duration STATE_TIMEOUT = 5_s;

static Duration last_precharge_low = 0_s;

motor_state precharge_state_next(motor_command cmd,
                                 Duration time_since_last_transition) {

  if (motor_command_DISCONNECT == cmd) {
    last_precharge_low = 0_s;
    return motor_state_IDLE;
  }

  if (time_since_last_transition > STATE_TIMEOUT) {
    last_precharge_low = 0_s;
    return motor_state_IDLE;
  }

  if (canzero_get_vdc45() < static_cast<float>(PRECHARGE_THRESH)) {
    last_precharge_low = time_since_last_transition;
  }

  if (time_since_last_transition - last_precharge_low > PRECHARGE_TIMETHRESH) {
    last_precharge_low = 0_s;
    return motor_state_READY;
  }

  pwm::disable_trig0_interrupt();
  pwm::disable_trig1_interrupt();
  pwm::disable_output();
  motor_board::set_sdc(true);

  return motor_state_PRECHARGE;
}
