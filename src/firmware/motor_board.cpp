#include "firmware/motor_board.h"
#include "InternalTemperature.h"
#include "core_pins.h"
#include "firmware/adc_etc.h"
#include "firmware/ain_scheduler.h"
#include "firmware/mux_scheduler.h"
#include "firmware/xbar.h"
#include <tuple>
#include "util/periodic_scheduler.h"
#include <SparkFunADXL313.h>

constexpr size_t MAX_AIN_PERIODIC_JOBS = 10;
constexpr size_t MAX_MUX_PERIODIC_JOBS = 8;

static AinScheduler<MAX_AIN_PERIODIC_JOBS> ain_scheduler;
static MuxScheduler<MAX_MUX_PERIODIC_JOBS> mux_scheduler;

void FLASHMEM motor_board::begin() {
  xbar::begin();
  pinMode(static_cast<uint8_t>(ctrl_pin::sdc_trig), OUTPUT);
  digitalWrite(static_cast<uint8_t>(ctrl_pin::sdc_trig), false);
  pinMode(static_cast<uint8_t>(ctrl_pin::precharge_start), OUTPUT);
  digitalWrite(static_cast<uint8_t>(ctrl_pin::precharge_start), false);
  pinMode(static_cast<uint8_t>(ctrl_pin::precharge_done), OUTPUT);
  digitalWrite(static_cast<uint8_t>(ctrl_pin::precharge_done), false);
}

Voltage FASTRUN motor_board::sync_read(ain_pin pin) {
  return adc_etc::read_single(pin);
}

Voltage FASTRUN motor_board::sync_read(mux_pin pin) {
  return mux_scheduler.sync_read(pin);
}

Temperature motor_board::read_mcu_temperature() {
  float temp = InternalTemperature.readTemperatureC();
  float temp_kelvin = temp + 273.15f;
  return Temperature(temp_kelvin);
}

void FASTRUN motor_board::mux_select(uint8_t sel) {
  digitalWrite(mux_sel0, sel & 0x1);
  digitalWrite(mux_sel1, sel & 0x2);
  digitalWrite(mux_sel2, sel & 0x4);
}

bool motor_board::register_periodic_reading(
    const Time &period, ain_pin pin, void (*on_value)(const Voltage &v)) {
  return ain_scheduler.schedule(period, AinSchedulerJob{
                                            .pin = pin,
                                            .on_value = on_value,
                                        });
}

bool FLASHMEM motor_board::register_periodic_reading(
    const Time &period, mux_pin pin, void (*on_value)(const Voltage &v)) {
  return mux_scheduler.register_periodic(period, pin, on_value);
}

void FASTRUN motor_board::delay(const Duration &dt) {
  delayMicroseconds(dt.as_us());
}

static ADXL313 adxl;
static void (*on_accelerometer_value)(const Acceleration &x,
                                      const Acceleration &y,
                                      const Acceleration &z);

bool FLASHMEM motor_board::register_periodic_accelerometer_reading(
    const Frequency &frequency, AccelerometerRange range,
    void (*on_value)(const Acceleration &x, const Acceleration &y,
                     const Acceleration &z)) {
  if (on_accelerometer_value != nullptr) {
    return false;
  }
  on_accelerometer_value = on_value;
  assert(adxl.beginSPI(ctrl_pin::accel_cs));
  adxl.setRange(range);
  adxl.setFullResBit(true);
  adxl.setFifoMode(ADXL313_FIFO_MODE_STREAM);
  adxl.setAxisOffset(0, 0, 0);
  adxl.setRate(static_cast<float>(frequency));
  adxl.clearFifo();
  adxl.measureModeOn();
  return true;
}

static constexpr Acceleration G = Acceleration(9.80665f);
static constexpr float RESOLUTION = 1024;

std::tuple<Acceleration, Acceleration, Acceleration>
    FASTRUN motor_board::sync_read_acceleration() {
  while (!adxl.dataReady()) {
  }
  adxl.readAccel();
  return std::make_tuple((adxl.x / RESOLUTION) * G, (adxl.y / RESOLUTION) * G,
                         (adxl.z / RESOLUTION) * G);
}

void FASTRUN motor_board::update() {

  mux_scheduler.update_continue();
  ain_scheduler.update_continue();

  if (on_accelerometer_value != nullptr) {
    if (adxl.dataReady()) {
      adxl.readAccel();
      on_accelerometer_value((adxl.x / RESOLUTION) * G,
                             (adxl.y / RESOLUTION) * G,
                             (adxl.z / RESOLUTION) * G);
    }
  }
}

void FASTRUN motor_board::set_digital(ctrl_pin pin, bool state) {
  digitalWrite(pin, state);
}

motor_board::InterruptLock FASTRUN motor_board::InterruptLock::acquire() {
  __disable_irq();
  return InterruptLock();
}

void FASTRUN motor_board::InterruptLock::release() {
  if (m_acquired) {
    __enable_irq();
  }
  m_acquired = false;
}
FASTRUN motor_board::InterruptLock::~InterruptLock() {
  if (m_acquired) {
    __enable_irq();
  }
  m_acquired = false;
}
