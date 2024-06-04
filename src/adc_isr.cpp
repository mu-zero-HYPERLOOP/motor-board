#include "adc_isr.h"
#include "control.h"
#include "firmware/adc_etc.h"
#include "firmware/motor_pwm.h"
#include "firmware/pwm.h"
#include "sensors/formula/current_sense.h"

void adc_etc_done0_isr(AdcTrigRes res) {
  const Voltage imeas_w2_0_0 = res.trig_res(TRIG0, 0);
  const Voltage imeas_v2_0_0 = res.trig_res(TRIG0, 1);
  const Voltage imeas_u2_0_0 = res.trig_res(TRIG0, 2);
  const Voltage imeas_w2_0_1 = res.trig_res(TRIG0, 3);
  const Voltage imeas_v2_0_1 = res.trig_res(TRIG0, 4);
  const Voltage imeas_u2_0_1 = res.trig_res(TRIG0, 5);

  const Voltage imeas_w1_0_0 = res.trig_res(TRIG4, 0);
  const Voltage imeas_v1_0_0 = res.trig_res(TRIG4, 1);
  const Voltage imeas_u1_0_0 = res.trig_res(TRIG4, 2);
  const Voltage imeas_w1_0_1 = res.trig_res(TRIG4, 3);
  const Voltage imeas_v1_0_1 = res.trig_res(TRIG4, 4);
  const Voltage imeas_u1_0_1 = res.trig_res(TRIG4, 5);

  const Voltage imeas_w1_0 = (imeas_w1_0_0 + imeas_w1_0_1) / 2.0f;
  const Voltage imeas_v1_0 = (imeas_v1_0_0 + imeas_v1_0_1) / 2.0f;
  const Voltage imeas_u1_0 = (imeas_u1_0_0 + imeas_u1_0_1) / 2.0f;
  const Voltage imeas_w2_0 = (imeas_w2_0_0 + imeas_w2_0_1) / 2.0f;
  const Voltage imeas_v2_0 = (imeas_v2_0_0 + imeas_v2_0_1) / 2.0f;
  const Voltage imeas_u2_0 = (imeas_u2_0_0 + imeas_u2_0_1) / 2.0f;

  const Current current_w1 = sensors::formula::current_sense(imeas_w1_0, 40, 0.2_mOhm);
  const Current current_v1 = sensors::formula::current_sense(imeas_v1_0, 40, 0.2_mOhm);
  const Current current_u1 = sensors::formula::current_sense(imeas_u1_0, 40, 0.2_mOhm);
  const Current current_w2 = sensors::formula::current_sense(imeas_w2_0, 40, 0.2_mOhm);
  const Current current_v2 = sensors::formula::current_sense(imeas_v2_0, 40, 0.2_mOhm);
  const Current current_u2 = sensors::formula::current_sense(imeas_u2_0, 40, 0.2_mOhm);

  const MotorPwmControl pwm_control = control::control_loop(
      current_w2, current_v2, current_u2, current_w1, current_v1, current_u1);

  pwm::control(static_cast<PwmControl>(pwm_control));
}

void adc_isr::begin() {

}

void adc_isr::calibrate() {

}

void adc_isr::update() {

}
