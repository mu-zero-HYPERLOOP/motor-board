#include "fsm/states.h"
#include "canzero/canzero.h"
#include "util/timestamp.h"

motor_state manual_state_next(motor_command cmd,
                            Duration time_since_last_transition) {
    return motor_state_MANUAL;
}
