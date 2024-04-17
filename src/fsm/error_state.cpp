#include "canzero.h"
#include "states.h"


motor_state error_state_next(motor_command cmd,
                            Duration time_since_last_transition) {
    return State::ready;
}
