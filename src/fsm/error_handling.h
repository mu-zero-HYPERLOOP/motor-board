#pragma once

#include "canzero.h"
#include "timestamp.h"

motor_command handle_errors(motor_state state, motor_command cmd, Duration time_since_last_transition);