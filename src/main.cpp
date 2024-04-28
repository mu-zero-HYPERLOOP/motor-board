#include "core_pins.h"
#include "firmware/Accelerometer.h"
#include "firmware/adc_etc.hpp"
#include "firmware/adc_mux.hpp"
#include "firmware/pinout.h"
#include "firmware/pwm.hpp"
#include "firmware/xbar.hpp"
#include "imxrt.h"
#include "pins_arduino.h"
#include "util/lina.h"
#include "wiring.h"
#include <Arduino.h>

/**
 * Talk to jakob about this example (He might now what PWM pins & ADC pins are
 * used)!
 */

volatile uint16_t imeas_w2_0, imeas_v2_0, imeas_u2_0;
volatile uint16_t imeas_w1_0, imeas_v1_0, imeas_u1_0;

volatile uint16_t imeas_w2_180, imeas_v2_180, imeas_u2_180;
volatile uint16_t imeas_w1_180, imeas_v1_180, imeas_u1_180;

void adc_etc_done0_isr(AdcTrigRes res) {
  imeas_w2_0 = res.trig_res<0, 0>();
  imeas_v2_0 = res.trig_res<0, 1>();
  imeas_u2_0 = res.trig_res<0, 2>();

  imeas_w1_0 = res.trig_res<4, 0>();
  imeas_v1_0 = res.trig_res<4, 1>();
  imeas_u1_0 = res.trig_res<4, 2>();
}

void adc_etc_done1_isr(AdcTrigRes res) {

  imeas_w2_180 = res.trig_res<1, 0>();
  imeas_v2_180 = res.trig_res<1, 1>();
  imeas_u2_180 = res.trig_res<1, 2>();

  imeas_w1_180 = res.trig_res<5, 0>();
  imeas_v1_180 = res.trig_res<5, 1>();
  imeas_u1_180 = res.trig_res<5, 2>();
}

void control() {}

int main() {
  Serial.begin(3000000);
  /* delay(5000); */
  xbar::begin();
  PwmBeginInfo beginInfo;
  beginInfo.enable_outputs = false;
  beginInfo.frequency = 20_kHz;
  beginInfo.deadtime = 10_ns;
  beginInfo.enable_trig1_interrupt = false;
  beginInfo.enable_trig0_interrupt = false;
  beginInfo.trig0 = 0.0f; // trig0 raised at reload
  //beginInfo.trig0 = 0.5f; // trig0 raised at center
  beginInfo.trig1 = std::nullopt;
  pwm::begin();

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
  int chain2_pins[3] = {AIN_I_MEAS_W1, AIN_I_MEAS_V1, AIN_I_MEAS_U1};
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

  AdcEtcBeginInfo adcBeginInfo = AdcEtcBeginInfo();
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

  xbar::connect(pwm::TRIG0_SIGNAL_SOURCE, AdcEtc::TRIG0_SIGNAL_SINK);
  xbar::connect(pwm::TRIG1_SIGNAL_SOURCE, AdcEtc::TRIG1_SIGNAL_SINK);

  /* Accelerometer::begin(); */

  while (true) {
    // low priority software read!

    uint16_t mux_readings[8];

    for (uint8_t sel = 0; sel < 8; sel++) {
      mux_readings[sel] = mux_read_blocking(sel);
    }

    /* const auto& [x,y,z] = Accelerometer::read_float_blocking(); */

    for (int i = 0; i < 80; i++) {
      Serial.printf("\n");
    }

    Serial.printf("========================================ADC================="
                  "======================\n");
    Serial.printf("-------------------0°----------Current-Measurement----------"
                  "-180°-----------------\n");
    Serial.printf("i_meas_w1[%2u]=%-5u| i_meas_w2[%2u]=%-5u | "
                  "i_meas_w1[%2u]=%-5u| i_meas_w2[%2u]=%-5u\n",
                  (uint32_t)AIN_I_MEAS_W1, imeas_w1_0, (uint32_t)AIN_I_MEAS_W2,
                  imeas_w2_0, (uint32_t)AIN_I_MEAS_W1, imeas_w1_180,
                  (uint32_t)AIN_I_MEAS_W2, imeas_w2_180);

    Serial.printf("i_meas_u1[%2u]=%-5u| i_meas_u2[%2u]=%-5u | "
                  "i_meas_u1[%2u]=%-5u| i_meas_u2[%2u]=%-5u\n",
                  (uint32_t)AIN_I_MEAS_U1, imeas_u1_0, (uint32_t)AIN_I_MEAS_U2,
                  imeas_u2_0, (uint32_t)AIN_I_MEAS_U1, imeas_u1_180,
                  (uint32_t)AIN_I_MEAS_U2, imeas_u2_180);

    Serial.printf("i_meas_v1[%2u]=%-5u| i_meas_v2[%2u]=%-5u | "
                  "i_meas_v1[%2u]=%-5u| i_meas_v2[%2u]=%-5u\n",
                  (uint32_t)AIN_I_MEAS_V1, imeas_v1_0, (uint32_t)AIN_I_MEAS_V2,
                  imeas_v2_0, (uint32_t)AIN_I_MEAS_V1, imeas_v1_180,
                  (uint32_t)AIN_I_MEAS_V2, imeas_v2_180);

    Serial.printf("------------------------------------Multiplexed-------------"
                  "----------------------\n");
    for (uint8_t sel = 0; sel < 8; sel++) {
      uint16_t avalue = mux_readings[sel];
      Serial.printf("mux%u=%-16u", sel, avalue);
      if (sel == 1 || sel == 5) {
        Serial.print(" ");
      }
      if (sel == 3) {
        Serial.printf("\n");
      }
    }
    Serial.printf("\n");
    /* Serial.printf("------------------------------------Accelerometer--------------------------------\n");
     */
    /* Serial.printf("accel_x = %u             accel_y = %u accel_z = %u\n",
     * x,y,z); */
    /*  */
    Serial.println("==========================================================="
                   "=======================");

    delay(50);
  }
}
