#include "fsm/fsm.h"

#include "firmware/Accelerometer.h"

int main() {

  Serial.begin(9600);
  delay(5000);

  Accelerometer::begin();

  while (true) {
    // acceleration readings (x,y,z are floats)
    const auto &[x, y, z] = Accelerometer::read_float_blocking();

    Serial.printf("{x = %f, y = %f, z = %f}\n", x, y, z);
  }
}
