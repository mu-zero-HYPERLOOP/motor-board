#include "canzero.h"
#include "states.h"

motor_state init_state_next(motor_command cmd,
                            Duration time_since_last_transition) {
                                
    return motor_state_IDLE;
}
