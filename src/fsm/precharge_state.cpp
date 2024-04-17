#include "canzero.h"
#include "states.h"

motor_state precharge_state_next(motor_command cmd,
                            Duration time_since_last_transition) {
    return motor_state_READY;
}
