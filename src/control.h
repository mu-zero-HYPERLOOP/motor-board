#pragma once
#include "firmware/motor_pwm.h"
#include "firmware/pwm.h"

namespace control {
void begin();

MotorPwmControl control_loop(Voltage vdc);


void update();

} // namespace control
