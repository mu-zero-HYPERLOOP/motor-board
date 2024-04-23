#include "canzero/canzero.h"
#include "fsm/states.h"

motor_state precharge_state_next(motor_command cmd,
                            Duration time_since_last_transition) {
    return motor_state_PRECHARGE;
}
