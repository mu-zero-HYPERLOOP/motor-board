#pragma once

#include "firmware/adc_etc.h"
#include "firmware/pwm.h"
#include "firmware/xbar.h"

static void xbar_config(){
  xbar::begin();

  xbar::connect(pwm::TRIG1_SIGNAL_SOURCE, adc_etc::TRIG0_SIGNAL_SINK);
}
