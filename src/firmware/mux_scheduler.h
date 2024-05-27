#pragma once
#include "firmware/motor_board.h"
#include "firmware/pinout.h"
#include "util/heap.h"
#include "util/metrics.h"
#include "util/pool_allocator.h"
#include <chrono>
#include <cstddef>

static std::chrono::high_resolution_clock::time_point SOR =
    std::chrono::high_resolution_clock::now();

template <size_t MAX_TASKS, typename Clock = std::chrono::high_resolution_clock>
struct MuxScheduler {
  using duration = typename Clock::duration;
  using time_point = typename Clock::time_point;

public:
  MuxScheduler() : m_mux_sel(0), m_last_mux_switch(Clock::now()) {
    motor_board::mux_select(m_mux_sel);
  }

  bool register_periodic(const Time &period, mux_pin pin,
                         void (*on_value)(const Voltage &)) {

    // parse pin!
    const uint8_t mux_sel = (pin & MUX_SEL_MASK) >> MUX_SEL_SHR;
    const ain_pin mux_pin = static_cast<ain_pin>(pin & MUX_PIN_MASK);

    const std::chrono::duration chrono_period =
        std::chrono::duration<float>(static_cast<float>(period));
    const duration clock_period = std::chrono::duration_cast<duration>(chrono_period);

    // Schedule new job!
    multiplexed_job *job = m_job_allocator.template alloc<multiplexed_job>();
    if (job == nullptr) {
      return false;
    }
    job->m_mux_sel = mux_sel;
    assert(mux_pin == ain_pin::ain_mux_21);
    job->m_on_ntc_value = on_value;
    job->m_ntc_period = clock_period;
    job->m_last_ntc_reading = Clock::now() - clock_period;

    return m_heap_scheduler.insert(Clock::now(), job);
  }

  void update_continue() {
    const auto &next_job = m_heap_scheduler.min();
    if (next_job.has_value()) {
      const auto [climax_ptr, multiplexed_job_ptr_ptr] = next_job.value();
      multiplexed_job *multiplexed_job_ptr = *multiplexed_job_ptr_ptr;

      if (m_mux_sel != multiplexed_job_ptr->m_mux_sel) {
        m_mux_sel = multiplexed_job_ptr->m_mux_sel;
        m_last_mux_switch = Clock::now();
        motor_board::mux_select(m_mux_sel);
      }
      const auto now = Clock::now();
      const auto time_since_last_switch = now - m_last_mux_switch;

      if (*climax_ptr < now &&
          time_since_last_switch > MIN_MUX_SWITCH_CHRONO_DURATION) {
        const Voltage v = motor_board::sync_read(ain_pin::ain_mux_21);
        multiplexed_job_ptr->m_on_ntc_value(v);
        const time_point next_climax = now + multiplexed_job_ptr->m_ntc_period;
        m_heap_scheduler.increase_key(0, next_climax);
      }
    }
  }

  Voltage sync_read(mux_pin pin) {

    const uint8_t mux_sel = (pin & MUX_SEL_MASK) >> MUX_SEL_SHR;
    const ain_pin mux_pin = static_cast<ain_pin>(pin & MUX_PIN_MASK);
    assert(mux_pin == ain_pin::ain_mux_21);

    const auto now = Clock::now();
    if (m_mux_sel != mux_sel) {
      m_mux_sel = mux_sel;
      motor_board::mux_select(mux_sel);
      m_last_mux_switch = now;
    }
    // delay for MIN_MUX_SWITCH_CHRONO_DURATION
    auto time_since_last_mux_switch = now - m_last_mux_switch;
    if (time_since_last_mux_switch <= MIN_MUX_SWITCH_CHRONO_DURATION) {
      auto time_until_mux_converges =
          MIN_MUX_SWITCH_CHRONO_DURATION - time_since_last_mux_switch;
      uint32_t us = std::chrono::duration_cast<
                        std::chrono::duration<unsigned int, std::micro>>(
                        time_until_mux_converges)
                        .count();
      motor_board::delay(Duration::from_us(us));
    }
    return motor_board::sync_read(mux_pin);
  }

public:
  static constexpr Time MIN_MUX_SWITCH_TIME = 10_ms;
  static constexpr duration MIN_MUX_SWITCH_CHRONO_DURATION =
      std::chrono::duration_cast<duration>(
          std::chrono::duration<float, std::ratio<1>>(
              static_cast<float>(MIN_MUX_SWITCH_TIME)));

private:
  // TODO custom pool allocator!
  struct multiplexed_job {
    uint8_t m_mux_sel;

    void (*m_on_ntc_value)(const Voltage &);
    duration m_ntc_period;
    time_point m_last_ntc_reading;
  };

  using Scheduler = MinHeap<time_point, multiplexed_job *, MAX_TASKS>;
  using Allocator = PoolAllocator<sizeof(multiplexed_job), MAX_TASKS>;

  uint8_t m_mux_sel;
  time_point m_last_mux_switch;

  Allocator m_job_allocator;
  Scheduler m_heap_scheduler;
};
