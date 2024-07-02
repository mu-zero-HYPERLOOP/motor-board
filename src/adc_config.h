#pragma once
#include "firmware/adc_etc.h"
#include "firmware/pinout.h"

static void adc_config() {

  TrigChainInfo adcEtcChains[1];
  adcEtcChains[0].trig_num = TRIG0;
  ain_pin chain0_pins[] = {ain_vdc_20};
  adcEtcChains[0].chain_length = sizeof(chain0_pins) / sizeof(ain_pin);
  adcEtcChains[0].read_pins = chain0_pins;
  adcEtcChains[0].chain_priority = 0;
  adcEtcChains[0].software_trig = false;
  adcEtcChains[0].trig_sync = false;
  adcEtcChains[0].intr = DONE0;

  AdcEtcBeginInfo adcBeginInfo = AdcEtcBeginInfo();
  adcBeginInfo.adc1_avg = HwAvg::SAMPLE_8;
  adcBeginInfo.adc1_clock_div = AdcClockDivider::NO_DIV;
  adcBeginInfo.adc1_high_speed = true;
  adcBeginInfo.adc1_sample_time = PERIOD_25;
  adcBeginInfo.adc2_avg = adcBeginInfo.adc1_avg;
  adcBeginInfo.adc2_clock_div = adcBeginInfo.adc1_clock_div;
  adcBeginInfo.adc2_high_speed = adcBeginInfo.adc1_high_speed;
  adcBeginInfo.adc2_sample_time = adcBeginInfo.adc1_sample_time;
  adcBeginInfo.num_chains = sizeof(adcEtcChains) / sizeof(TrigChainInfo);
  adcBeginInfo.chains = adcEtcChains;

  adc_etc::begin(adcBeginInfo);
}
