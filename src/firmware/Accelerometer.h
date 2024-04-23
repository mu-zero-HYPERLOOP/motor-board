#pragma once 
#include "util/metrics.h"
#include <SPI.h>
#include <SparkFunADXL313.h>
#include <cassert>

constexpr uint8_t ACCELEROMETER_CHIP_SELECT_PIN = 10;

class Accelerometer {
public:
  static void begin();

  static std::tuple<float, float, float> read_float_blocking();
  static std::tuple<Acceleration, Acceleration, Acceleration> read_blocking();

private:
  static ADXL313 s_adxl;
};
