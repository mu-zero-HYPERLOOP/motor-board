#pragma once

#include "util/metrics.h"
namespace sensors::formula {

static Temperature ptxxx(Resistance r_ntc, Resistance r0, float alpha) {
  return Temperature(static_cast<float>(r_ntc));
}

static Resistance inv_ptxx(Temperature temp, Resistance r0, float alpha) {
  return Resistance(static_cast<float>(temp));
}

} // namespace sensors::formula
