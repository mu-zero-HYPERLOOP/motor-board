#include "control.h"
#include "firmware/motor_pwm.h"

PwmBeginInfo control::begin() {
  PwmBeginInfo pwmBeginInfo;
  pwmBeginInfo.enable_outputs = false;
  pwmBeginInfo.frequency = 20_kHz;
  pwmBeginInfo.deadtime = 10_ns;
  // the control loops gets called from the adc_etc isr not the pwm reload isr.
  pwmBeginInfo.enable_trig1_interrupt = false;
  pwmBeginInfo.enable_trig0_interrupt = false;
  // the triggers are forwarded to the adc_etc (see firmware)
  pwmBeginInfo.trig0 = 0.0f; // trig0 raised at reload
  pwmBeginInfo.trig1 = std::nullopt;
  return pwmBeginInfo;
}

MotorPwmControl control::control_loop(Current current_w2, Current current_v2,
                                      Current current_u2, Current current_w1,
                                      Current current_v1, Current current_u1) {
  MotorPwmControl control;
  control.W1_duty = 0.5;
  control.W2_duty = 0.5;
  control.V1_duty = 0.5;
  control.V2_duty = 0.5;
  control.U1_duty = 0.5;
  control.U2_duty = 0.5;
  return control;
}
