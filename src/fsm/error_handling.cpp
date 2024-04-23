#include "fsm/error_handling.h"
#include "canzero/canzero.h"
#include "util/timestamp.h"

motor_command handle_errors(motor_state state, motor_command cmd,
                             Duration time_since_last_transition) {

    return cmd;
}
