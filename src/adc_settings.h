#pragma once
#include "firmware/adc_etc.hpp"

namespace adc_settings {

static AdcEtcBeginInfo begin() {

  TrigChainInfo adcEtcChains[4];
  // see xbar_init() for when this trig0 signal gets raised
  adcEtcChains[0].trig_num = TRIG0;
  ain_pin chain1_pins[3] = {ain_imeas_w2_19, ain_imeas_v2_18, ain_imeas_u2_17};
  adcEtcChains[0].chain_length = sizeof(chain1_pins) / sizeof(int);
  adcEtcChains[0].read_pins = chain1_pins;
  adcEtcChains[0].chain_priority = 0;
  adcEtcChains[0].software_trig = false;
  adcEtcChains[0].trig_sync = true;
  adcEtcChains[0].intr = DONE0;

  adcEtcChains[1].trig_num = TRIG4;
  ain_pin chain2_pins[3] = {ain_imeas_w1_14, ain_imeas_v1_15, ain_imeas_u1_16};
  adcEtcChains[1].chain_length = sizeof(chain2_pins) / sizeof(int);
  adcEtcChains[1].read_pins = chain2_pins;
  adcEtcChains[1].chain_priority = 0;
  adcEtcChains[1].software_trig = false;
  adcEtcChains[1].trig_sync = false;
  adcEtcChains[1].intr = NONE;

  adcEtcChains[2].trig_num = TRIG1;
  ain_pin chain3_pins[3] = {ain_imeas_w2_19, ain_imeas_v2_18, ain_imeas_u2_17};
  adcEtcChains[2].chain_length = sizeof(chain3_pins) / sizeof(int);
  adcEtcChains[2].read_pins = chain3_pins;
  adcEtcChains[2].chain_priority = 0;
  adcEtcChains[2].software_trig = false;
  adcEtcChains[2].trig_sync = true;
  adcEtcChains[2].intr = DONE1;

  adcEtcChains[3].trig_num = TRIG5;
  ain_pin chain4_pins[3] = {ain_imeas_w1_14, ain_imeas_v1_15, ain_imeas_u1_16};
  adcEtcChains[3].chain_length = sizeof(chain4_pins) / sizeof(int);
  adcEtcChains[3].read_pins = chain4_pins;
  adcEtcChains[3].chain_priority = 0;
  adcEtcChains[3].software_trig = false;
  adcEtcChains[3].trig_sync = false;
  adcEtcChains[3].intr = NONE;

  AdcEtcBeginInfo adcBeginInfo = AdcEtcBeginInfo();
  adcBeginInfo.adc1_avg = HwAvg::SAMPLE_4;
  adcBeginInfo.adc1_clock_div = AdcClockDivider::NO_DIV;
  adcBeginInfo.adc1_high_speed = true;
  adcBeginInfo.adc1_sample_time = PERIOD_25;
  adcBeginInfo.adc2_avg = adcBeginInfo.adc1_avg;
  adcBeginInfo.adc2_clock_div = adcBeginInfo.adc1_clock_div;
  adcBeginInfo.adc2_high_speed = adcBeginInfo.adc2_high_speed;
  adcBeginInfo.adc2_sample_time = adcBeginInfo.adc2_sample_time;
  adcBeginInfo.num_chains = sizeof(adcEtcChains) / sizeof(TrigChainInfo);
  adcBeginInfo.chains = adcEtcChains;

  return adcBeginInfo;
}

}
