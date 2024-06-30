#include "adc_isr.h"
#include "control.h"
#include "firmware/adc_etc.h"
#include "firmware/motor_pwm.h"
#include "firmware/pwm.h"
#include "sensors/formula/current_sense.h"
#include "sensors/formula/isolated_voltage.h"

void adc_etc_done0_isr(AdcTrigRes res) {
  /* const Voltage vdc_v_meas = res.trig_res(TRIG0, 0); */
  /* sensors::formula::isolated_voltage(vdc_v_meas); */

  const MotorPwmControl pwm_control = control::control_loop(0_V);

  pwm::control(static_cast<PwmControl>(pwm_control));
}

void adc_isr::begin() {

}

void adc_isr::calibrate() {

}

void adc_isr::update() {

}
