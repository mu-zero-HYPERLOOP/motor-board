#include "fsm/fsm.h"
#include "canzero/canzero.h"

#include "fsm/error_handling.h"
#include "fsm/states.h"
#include "util/timestamp.h"

Timestamp g_fsm_last_transition = Timestamp::now();

void fsm::begin() {
  g_fsm_last_transition = Timestamp::now();
  canzero_set_state(motor_state_INIT);
}

void fsm::update() {
    Timestamp now = Timestamp::now();
    Duration time_since_last_transition = now - g_fsm_last_transition;

    motor_state state = canzero_get_state();
    motor_command cmd =
        handle_errors(state, canzero_get_command(), time_since_last_transition);

    motor_state next_state;
    switch(state) {
        case motor_state_INIT:
            next_state = init_state_next(cmd, time_since_last_transition);
            break;
        case motor_state_IDLE:
            next_state = idle_state_next(cmd, time_since_last_transition);
            break;
        case motor_state_PRECHARGE:
            next_state = precharge_state_next(cmd, time_since_last_transition);
            break;
        case motor_state_READY:
            next_state = ready_state_next(cmd, time_since_last_transition);
            break;
        case motor_state_ACCELERATE:
            next_state = accelerate_state_next(cmd, time_since_last_transition);
            break;
        case motor_state_CONTROL:
            next_state = control_state_next(cmd, time_since_last_transition);
            break;
        case motor_state_DECELERATE:
            next_state = decelerate_state_next(cmd, time_since_last_transition);
            break;
        case motor_state_MANUAL:
            next_state = manual_state_next(cmd, time_since_last_transition);
            break;
        case motor_state_ERROR:
            next_state = error_state_next(cmd, time_since_last_transition);
            break;
    }

    if (next_state != state) {
        g_fsm_last_transition = now;
        canzero_set_state(next_state);
    }
}
