#include "canzero.h"
#include "timestamp.h"

motor_state idle_state_next(motor_command cmd,
                            Duration time_since_last_transition) {

    if (motor_command_PRECHARGE == cmd) {
        return motor_state_PRECHARGE;
    } else {
        //Actions
        return motor_state_IDLE;
    }
}
