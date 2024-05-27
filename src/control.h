#include "firmware/motor_pwm.h"
#include "firmware/pwm.hpp"

namespace control {
PwmBeginInfo begin();

MotorPwmControl control_loop(Current current_w2, Current current_v2,
                       Current current_u2, Current current_w1,
                       Current current_v1, Current current_u1);
} // namespace control
