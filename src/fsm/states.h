#pragma once

#include "canzero.h"
#include "timestamp.h"

motor_state init_state_next(motor_command cmd, Duration time_since_last_transition);

motor_state idle_state_next(motor_command cmd, Duration time_since_last_transition);

motor_state precharge_state_next(motor_command cmd, Duration time_since_last_transition);

motor_state ready_state_next(motor_command cmd, Duration time_since_last_transition);

motor_state start_state_next(motor_command cmd, Duration time_since_last_transition);

motor_state control_state_next(motor_command cmd, Duration time_since_last_transition);

motor_state stop_state_next(motor_command cmd, Duration time_since_last_transition);

motor_state manual_state_next(motor_command cmd, Duration time_since_last_transition);

motor_state error_state_next(motor_command cmd, Duration time_since_last_transition);