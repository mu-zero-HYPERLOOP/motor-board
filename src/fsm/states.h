#pragma once

#include "canzero/canzero.h"
#include "util/timestamp.h"

namespace fsm::states {

motor_state init(motor_command cmd,
                            Duration time_since_last_transition);

motor_state idle(motor_command cmd,
                            Duration time_since_last_transition);

motor_state arming45(motor_command cmd,
                            Duration time_since_last_transition);

motor_state precharge(motor_command cmd,
                                 Duration time_since_last_transition);

motor_state ready(motor_command cmd,
                             Duration time_since_last_transition);

motor_state control(motor_command cmd,
                                  Duration time_since_last_transition);

motor_state disarming45(motor_command cmd,
                                  Duration time_since_last_transition);

} // namespace fsm::states
