#include "Accelerometer.h"

#include "pinout.h"


ADXL313 Accelerometer::s_adxl;
void Accelerometer::begin() {
  if (s_adxl.beginSPI(ACCEL_CS) == false) {
    Serial.println("Failed to detect ADXL313 Accelerometer");
    /* assert(false); */
  }
  s_adxl.measureModeOn();
}

std::tuple<float, float, float> Accelerometer::read_float_blocking() {
  while (!s_adxl.dataReady()) {
  }
  s_adxl.readAccel();
  const float &x = s_adxl.x;
  const float &y = s_adxl.y;
  const float &z = s_adxl.z;

  return {x, y, z};
}

std::tuple<Acceleration, Acceleration, Acceleration>
Accelerometer::read_blocking() {
  const auto &[x, y, z] = read_float_blocking();
  return {Acceleration(x), Acceleration(y), Acceleration(z)};
}
