#pragma once

#include "util/metrics.h"
#include <cmath>
namespace sensors::formula {

static constexpr float ntc_beta_from_ref(Resistance R1, Temperature T1, Resistance R2, Temperature T2) {
  return static_cast<float>(std::log(R1 / R2) / ((1.0f / T1) - 1.0f / T2));
}

static constexpr Temperature ntc_beta(Resistance R, Temperature beta, Resistance r_ref, Temperature t_ref) {
  return 1.0f / (std::log(R / r_ref) / beta + 1.0f / t_ref);
}

static constexpr Resistance inv_ntc_beta(Temperature T, Temperature beta, Resistance r_ref, 
    Temperature t_ref) {
  return r_ref * std::exp(beta / T - beta / t_ref);
}

}
