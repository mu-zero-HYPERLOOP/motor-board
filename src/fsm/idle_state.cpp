#include "canzero/canzero.h"
#include "firmware/motor_board.h"
#include "util/timestamp.h"
#include "firmware/pwm.hpp"

motor_state idle_state_next(motor_command cmd,
                            Duration time_since_last_transition) {

    if (motor_command_PRECHARGE == cmd) {
        return motor_state_PRECHARGE;
    } 

    pwm::disable_trig0_interrupt();
    pwm::disable_trig1_interrupt();
    pwm::disable_output();
    motor_board::set_sdc(false);

    return motor_state_IDLE;
}
