#pragma once

#include "util/metrics.h"
namespace sensors::formula{

static inline Voltage isolated_voltage(Voltage v) {
  return v * ((51000 + 275) / (275 * 1.5 * 0.4));
}

static inline Voltage inv_isolated_voltage(Voltage v) {
  return v / (52500 / (1500 * 1.5 * 0.4));
}


}
