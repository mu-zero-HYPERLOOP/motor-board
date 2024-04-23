#include "canzero/canzero.h"
#include "fsm/states.h"

motor_state start_state_next(motor_command cmd,
                            Duration time_since_last_transition) {
    return motor_state_START;
}
