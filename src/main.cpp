#include "fsm/fsm.h"

#include "firmware/Accelerometer.h"

int main() {

  Accelerometer::begin();

  while (true) {
    // acceleration readings (x,y,z are floats)
    const auto &[x, y, z] = Accelerometer::read_float_blocking();

    Serial.printf("{x = %f, y = %f, z = %f}\n", x, y, z);
  }
}
