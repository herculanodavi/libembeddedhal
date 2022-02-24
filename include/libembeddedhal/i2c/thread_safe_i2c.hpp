#pragma once

#include "i2c.hpp"
#include <mutex>

namespace embed::thread_safe {
class i2c : public embed::i2c
{
public:
  i2c(embed::i2c& p_i2c)
    : m_i2c(&p_i2c)
  {}

  boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept override
  {
    m_mutex.lock();
    auto result = m_i2c->configure(p_settings);
    m_mutex.unlock();
    return result;
  }

  boost::leaf::result<void> driver_transaction(
    std::byte p_address,
    std::span<const std::byte> p_data_out,
    std::span<std::byte> p_data_in) noexcept override
  {
    m_mutex.lock();
    auto result = m_i2c->transaction(p_address, p_data_out, p_data_in);
    m_mutex.unlock();
    return result;
  }

private:
  std::mutex m_mutex;
  embed::i2c* m_i2c;
};
}  // namespace embed::thread_safe

class numchuck
{
public:
  numchuck(embed::i2c& p_i2c)
    : m_i2c(&p_i2c)
  {}

  struct data
  {
    // fill up with numchuck data
  };

  data get()
  {
    m_i2c->transaction(std::byte{ 0x9a }, {}, {});
    return {};
  }

  embed::i2c* m_i2c;
};

int main()
{
  embed::lpc40xx::i2c& i2c1 = embed::lpc40xx::i2c::get<1>();
  embed::thread_safe::i2c thread_safe_i2c1(i2c1);

  numchuck num(i2c1);
  num.get();
  numchuck threadsafe_num(thread_safe_i2c1);
  threadsafe_num.get();
  lcd_screen lcd(thread_safe_i2c1);
}
