#pragma once

#include <queue>

#include "interface.hpp"

namespace embed::mock {
/**
 * @brief mock accelerometer implementation for use in unit tests and
 * simulations.
 *
 */
struct accelerometer : public embed::accelerometer
{
  /**
   * @brief Queues the samples to be returned for read()
   *
   * @param p_samples - queue of samples
   */
  void set(std::queue<sample>& p_samples)
  {
    m_samples = p_samples;
  }

private:
  boost::leaf::result<sample> driver_read() noexcept
  {
    if (m_samples.size() == 0) {
      return boost::leaf::new_error(
        std::out_of_range("samples queue is empty!"));
    }
    sample m_current_value = m_samples.front();
    m_samples.pop();
    return m_current_value;
  }

  std::queue<sample> m_samples{};
};
}  // namespace embed::mock
