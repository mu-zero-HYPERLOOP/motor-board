#include "core_pins.h"
#include "firmware/motor_pwm.hpp"
#include "fsm/fsm.h"
#include <Arduino.h>

#include "pins_arduino.h"

constexpr float PWM_FREQUENCY = 20000;

int main() {

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  delay(5000);

  pwm_init();
  digitalWrite(LED_BUILTIN, HIGH);

  pwm_control_t pwm_control;

  pwm_control.V1_duty = 0.5;
  pwm_control.V2_duty = 0.5;
  pwm_control.W1_duty = 0.5;
  pwm_control.W2_duty = 0.5;

  pwm_set_control(pwm_control);

  set_PWM_freq(4000);

  Serial.println((int16_t)FLEXPWM4_SM2CTRL);
  Serial.println("---");
  delay(2000);

  set_PWM_freq(2000);
  Serial.println((int16_t)FLEXPWM4_SM2CTRL);
  Serial.println("---");

  delay(2000);

  set_PWM_freq(4000);
  Serial.println((int16_t)FLEXPWM4_SM2CTRL);
  Serial.println("---");

  digitalWrite(LED_BUILTIN, LOW);

  while (true) {
    digitalWrite(LED_BUILTIN, HIGH);

    pwm_control.W2_duty += 0.01;
    if (pwm_control.W2_duty > 1) {
      pwm_control.W2_duty = 0;
      set_PWM_freq(PWM_FREQUENCY);
    }

    pwm_set_control(pwm_control);

    delay(10);

    digitalWrite(LED_BUILTIN, LOW);
    delay(10);
  }
}
