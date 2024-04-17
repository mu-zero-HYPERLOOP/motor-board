#include "canzero.h"
#include "states.h"

motor_state ready_state_next(motor_command cmd,
                            Duration time_since_last_transition) {

    if (cmd == "TODO") {
        return motor_state_START;
    } else {
        return motor_state_READY;
    }
}
