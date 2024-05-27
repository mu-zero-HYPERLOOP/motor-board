#include "firmware/adc_etc.hpp"
#include "firmware/motor_pwm.h"
#include "control.h"

static Current formula_current_measurement(Voltage v) {
  // TODO current sensor voltage to current convertion
  return 0_A;
}

void adc_etc_done0_isr(AdcTrigRes res) {
  /* const float imeas_w2_0 = res.trig_res(0, 0); */
  /* const float imeas_v2_0 = res.trig_res(0, 1); */
  /* const float imeas_u2_0 = res.trig_res(0, 2); */
  /*  */
  /* const float imeas_w1_0 = res.trig_res(4, 0); */
  /* const float imeas_v1_0 = res.trig_res(4, 1); */
  /* const float imeas_u1_0 = res.trig_res(4, 2); */
  const Current current_w2 = formula_current_measurement(0_V);
  const Current current_v2 = formula_current_measurement(0_V);
  const Current current_u2 = formula_current_measurement(0_V);
  const Current current_w1 = formula_current_measurement(0_V);
  const Current current_v1 = formula_current_measurement(0_V);
  const Current current_u1 = formula_current_measurement(0_V);

  const MotorPwmControl pwm_control = control::control_loop(
      current_w2, current_v2, current_u2, current_w1, current_v1, current_u1);

  pwm::control(pwm_control);
}

void adc_etc_done1_isr(AdcTrigRes res) {}
