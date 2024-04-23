#include "canzero/canzero.h"
#include "fsm/states.h"

motor_state ready_state_next(motor_command cmd,
                            Duration time_since_last_transition) {

  return motor_state_READY;
}
