#pragma once
#include "firmware/motor_pwm.h"
#include "firmware/pwm.h"

namespace control {
void begin();

MotorPwmControl control_loop(Current current_w2, Current current_v2,
                       Current current_u2, Current current_w1,
                       Current current_v1, Current current_u1);


void update();

} // namespace control
