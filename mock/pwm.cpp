#include "firmware/pwm.hpp"

void pwm::enable_output() {
  if (m_outen)
    return;
  m_outen = true;
}

void pwm::disable_output() {
  if (!m_outen)
    return;
  m_outen = false;
}

void pwm::trig0(const std::optional<float> &trig0) {
  if (m_trig0 == trig0) {
    return;
  }
  m_trig0 = trig0;
}

void pwm::trig1(const std::optional<float> &trig1) {
  if (m_trig1 == trig1) {
    return;
  }
  m_trig1 = trig1;
}

void pwm::enable_trig0_interrupt() {
  if (m_trig0_inten) {
    return;
  }
  m_trig0_inten = true;
}

void pwm::disable_trig0_interrupt() {
  if (!m_trig0_inten) {
    return;
  }
  m_trig0_inten = false;
}

void pwm::enable_trig1_interrupt() {
  if (m_trig1_inten) {
    return;
  }
  m_trig1_inten = true;
}

void pwm::disable_trig1_interrupt() {
  if (!m_trig1_inten) {
    return;
  }
  m_trig1_inten = false;
}

void pwm::frequency(const Frequency &frequency) {
}

void pwm::write_control() {

}

void pwm::deadtime(const Time &deadtime) {
}

void pwm::control(const PwmControl &control) {
}

void pwm::begin(const PwmBeginInfo &beginInfo) {
}

Frequency pwm::m_frequency;
Time pwm::m_deadtime;
PwmControl pwm::m_control;
bool pwm::m_outen;
bool pwm::m_trig0_inten;
bool pwm::m_trig1_inten;
std::optional<float> pwm::m_trig0;
std::optional<float> pwm::m_trig1;
volatile uint32_t pwm::m_pwm_cycles;
volatile uint32_t pwm::m_deadtime_cycles;

int pwm::TRIG0_SIGNAL_SOURCE = 0; // mocked
int pwm::TRIG1_SIGNAL_SOURCE = 0; // mocked
