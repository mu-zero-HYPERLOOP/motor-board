#include "core_pins.h"
#include "firmware/adc_etc.hpp"
#include "firmware/adc_mux.hpp"
#include "firmware/motor_pwm.hpp"
#include "firmware/pinout.h"
#include "firmware/xbar.hpp"
#include "firmware/Accelerometer.h"
#include "imxrt.h"
#include "pins_arduino.h"
#include "wiring.h"
#include <Arduino.h>
#include "util/lina.h"
#include "util/metrics.h"

/**
 * Talk to jakob about this example (He might now what PWM pins & ADC pins are
 * used)!
 */

volatile uint16_t imeas_w2_0, imeas_v2_0, imeas_u2_0;
volatile uint16_t imeas_w1_0, imeas_v1_0, imeas_u1_0;
volatile float imeas_bat;
bool error_flag = false;

volatile uint16_t imeas_w2_180, imeas_v2_180, imeas_u2_180;
volatile uint16_t imeas_w1_180, imeas_v1_180, imeas_u1_180;

constexpr float sqrt3over2 = 0.866025;

// some values for testing
constexpr Frequency my_pwm_freq = 20_kHz;
volatile Frequency rotational_freq = 32_Hz;
float modulation_index = 0.6;

int main_counter = 0;


void adc_etc_done0_isr(AdcTrigRes res) {
  if(modulation_index > 0.8) {
    modulation_index = 0;
  }
  if(!error_flag) {
    imeas_w2_0 = res.trig_res<0, 0>();
    imeas_v2_0 = res.trig_res<0, 1>();
    imeas_u2_0 = res.trig_res<0, 2>();

    imeas_w1_0 = res.trig_res<4, 0>();
    imeas_v1_0 = res.trig_res<4, 1>();
    imeas_u1_0 = res.trig_res<4, 2>();

    imeas_bat = 0.99 * imeas_bat + 0.01 * ((3.3 * res.trig_res<4,3>() / 4096.0 / 1.5 / 0.4 - 2.5) / 50 / 0.0001 + 2);

    pwm_control_t isr_control;

    static float theta = 0;
    theta += rotational_freq * TWO_PI / my_pwm_freq;
    if(theta > TWO_PI) {
      theta -= TWO_PI;
    }

    // complex pointer u_a + j*u_b
    float u_a = modulation_index * cos(theta) * sqrt3over2 / 2;
    float u_b = modulation_index * sin(theta) * sqrt3over2 / 2;

    float u_u = u_a;
    float u_v = -0.5 * u_a + sqrt3over2 * u_b;
    float u_w = -0.5 * u_a - sqrt3over2 * u_b;
    if(false) {
      isr_control.U1_duty = 0;
      isr_control.V1_duty = 0;
      isr_control.W1_duty = 0;
      error_flag = true;

    } else {
      isr_control.U1_duty = 0.5 + u_u/1.0; 
      isr_control.V1_duty = 0.5 + u_v/1.0; 
      isr_control.W1_duty = 0.5 + u_w/1.0; 
    }

    pwm_set_control(isr_control);
  }
}

void adc_etc_done1_isr(AdcTrigRes res) {

  imeas_w2_180 = res.trig_res<1, 0>();
  imeas_v2_180 = res.trig_res<1, 1>();
  imeas_u2_180 = res.trig_res<1, 2>();

  imeas_w1_180 = res.trig_res<5, 0>();
  imeas_v1_180 = res.trig_res<5, 1>();
  imeas_u1_180 = res.trig_res<5, 2>();
}

void control() {
  
}

int main() {
  Serial.begin(9600);
  /* delay(5000); */
  xbar_init();
  pwm_init();
  set_PWM_freq(static_cast<float>(my_pwm_freq));

  pinMode(InverterPin::SDC_TRIG, OUTPUT);
  pinMode(InverterPin::PRECHARGE_START, OUTPUT);
  pinMode(InverterPin::PRECHARGE_DONE, OUTPUT);

  TrigChainInfo chains[4];
  // see xbar_init() for when this trig0 signal gets raised
  chains[0].trig_num = TRIG0;
  int chain1_pins[3] = {AIN_I_MEAS_W2, AIN_I_MEAS_V2, AIN_I_MEAS_U2};
  chains[0].chain_length = sizeof(chain1_pins) / sizeof(int);
  chains[0].read_pins = chain1_pins;
  chains[0].chain_priority = 0;
  chains[0].software_trig = false;
  chains[0].trig_sync = true;
  chains[0].intr = DONE0;

  chains[1].trig_num = TRIG4;
  int chain2_pins[4] = {AIN_I_MEAS_W1, AIN_I_MEAS_V1, AIN_I_MEAS_U1, AIN_I_MEAS_BAT};
  chains[1].chain_length = sizeof(chain2_pins) / sizeof(int);
  chains[1].read_pins = chain2_pins;
  chains[1].chain_priority = 0;
  chains[1].software_trig = false;
  chains[1].trig_sync = false; // <-- check me (maybe just try true as well)
  chains[1].intr = NONE;

  chains[2].trig_num = TRIG1;
  int chain3_pins[3] = {AIN_I_MEAS_W2, AIN_I_MEAS_V2, AIN_I_MEAS_U2};
  chains[2].chain_length = sizeof(chain3_pins) / sizeof(int);
  chains[2].read_pins = chain3_pins;
  chains[2].chain_priority = 0;
  chains[2].software_trig = false;
  chains[2].trig_sync = true;
  chains[2].intr = DONE1;

  chains[3].trig_num = TRIG5;
  int chain4_pins[3] = {AIN_I_MEAS_W1, AIN_I_MEAS_V1, AIN_I_MEAS_U1};
  chains[3].chain_length = sizeof(chain4_pins) / sizeof(int);
  chains[3].read_pins = chain4_pins;
  chains[3].chain_priority = 0;
  chains[3].software_trig = false;
  chains[3].trig_sync = false; // <-- check me (maybe just try true as well)
  chains[3].intr = NONE;

  AdcEtcBeginInfo adcBeginInfo;
  adcBeginInfo.adc1_avg = HwAvg::SAMPLE_4;
  adcBeginInfo.adc1_clock_div = AdcClockDivider::NO_DIV;
  adcBeginInfo.adc1_high_speed = true;
  adcBeginInfo.adc1_sample_time = PERIOD_25;
  adcBeginInfo.adc2_avg = adcBeginInfo.adc1_avg;
  adcBeginInfo.adc2_clock_div = adcBeginInfo.adc1_clock_div;
  adcBeginInfo.adc2_high_speed = adcBeginInfo.adc2_high_speed;
  adcBeginInfo.adc2_sample_time = adcBeginInfo.adc2_sample_time;
  adcBeginInfo.num_chains = sizeof(chains) / sizeof(TrigChainInfo);
  adcBeginInfo.chains = chains;
  AdcEtc::begin(adcBeginInfo);


  delay(5000);
  Serial.println("Activate 45V");
  delay(5000);

  digitalWrite(InverterPin::SDC_TRIG, HIGH);
  delay(1000);
  digitalWrite(InverterPin::PRECHARGE_START, HIGH);
  delay(2000);
  digitalWrite(InverterPin::PRECHARGE_DONE, HIGH);
  delay(500);
  digitalWrite(InverterPin::PRECHARGE_START, LOW);
  delay(1000);
  

  /* Accelerometer::begin(); */

  while (true) {
    // low priority software read!

    uint16_t mux_readings[8];

    for (uint8_t sel = 0; sel < 8; sel++) {
      mux_readings[sel] = mux_read_blocking(sel);
    }

    /* const auto& [x,y,z] = Accelerometer::read_float_blocking(); */

    // rotational_freq = rotational_freq + 0.5_Hz;
    Serial.printf("freq: %f  -  i_bat: %f \n", static_cast<float>(rotational_freq), imeas_bat);

    if(main_counter > 100) {
      // rotational_freq = 2_Hz;
      main_counter = 0;
    }

    main_counter++;

    delay(100);
  }
}
