#include "fsm/fsm.h"
#include "canzero/canzero.h"
#include "fsm/error_handling.h"
#include "fsm/states.h"
#include "util/timestamp.h"

static Timestamp fsm_last_transition = Timestamp::now();

void fsm::begin() {
  fsm_last_transition = Timestamp::now();
  canzero_set_state(motor_state_INIT);
  canzero_set_command(motor_command_NONE);
  canzero_set_error_arming_failed(error_flag_OK);
  canzero_set_error_precharge_failed(error_flag_OK);
  canzero_update_continue(canzero_get_time());
}

void fsm::finish_init() {
  canzero_set_state(motor_state_IDLE);
  canzero_update_continue(canzero_get_time());
}

void fsm::update() {

  motor_state state;
  motor_state next_state;
  do {

    Timestamp now = Timestamp::now();
    Duration time_since_last_transition = now - fsm_last_transition;

    motor_command cmd = error_handling::approve(canzero_get_command());

    state = canzero_get_state();
    switch (state) {
    case motor_state_INIT:
      next_state = states::init(cmd, time_since_last_transition);
      break;
    case motor_state_IDLE:
      next_state = states::idle(cmd, time_since_last_transition);
      break;
    case motor_state_ARMING45:
      next_state = states::arming45(cmd, time_since_last_transition);
      break;
    case motor_state_PRECHARGE:
      next_state = states::precharge(cmd, time_since_last_transition);
      break;
    case motor_state_READY:
      next_state = states::ready(cmd, time_since_last_transition);
      break;
    case motor_state_CONTROL:
      next_state = states::control(cmd, time_since_last_transition);
      break;
    case motor_state_DISARMING45:
      next_state = states::disarming45(cmd, time_since_last_transition);
      break;
    }

    if (next_state != state) {
      fsm_last_transition = now;
      canzero_set_state(next_state);
      canzero_update_continue(canzero_get_time());
    }
  } while (next_state != state);
}
