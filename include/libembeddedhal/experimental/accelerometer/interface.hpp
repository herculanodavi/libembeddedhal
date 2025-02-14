#pragma once

#include <cstdint>

#include "../error.hpp"
#include "../percent.hpp"
#include "unit.hpp"

namespace embed {
/**
 * @brief Accelerometer hardware abstraction interface.
 *
 * This interface represents devices and peripherals that have the capability to
 * measure acceleration.
 *
 */
class accelerometer
{
public:
  /// Represents the percentage of acceleration in the X, Y & Z axis relative
  /// to the full-scale
  struct axis_t
  {
    /// Percentage of acceleration in the X-axis relative to the full-scale
    percent x;
    /// Percentage of acceleration in the Y-axis relative to the full-scale
    percent y;
    /// Percentage of acceleration in the Z-axis relative to the full-scale
    percent z;

    /**
     * @brief Default operators for <, <=, >, >= and ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator<=>(const axis_t&) const noexcept =
      default;
  };

  /**
   * @brief Representation of a sample of accelerometer data
   *
   * embed::percent was chosen for representing the x, y, and z data of an
   * accelerometer because conversion from N-bit register to percent is fast
   * compared to the math required to convert it to
   * embed::nanometer_per_second_sq. This means that multiple samples can be
   * gathered quickly to be converted to data at a later time.
   *
   */
  struct sample
  {
    /// Represents the maximum amplitude of acceleration for this sample of
    /// data. In many cases this is 1g, 2g, 4g, 8g and many other values, where
    /// "g" represents the acceleration due to gravity on Earth at sea level.
    /// For example, if full_scale is 2g and the X-axis returns 50%, then the
    /// acceleration read on X is 1g of acceleration.
    ///
    /// full_scale is made available for each sample in the event that the
    /// driver changes the full_scale between samples. This is uncommon and many
    /// applications will simply save the full scale once and drop saving it for
    /// subsequent calls.
    embed::microgravity full_scale;
    /// Acceleration in the XYZ axis
    axis_t axis;
    /**
     * @brief Default operators for <, <=, >, >= and ==
     *
     * @return auto - result of the comparison
     */
    [[nodiscard]] constexpr auto operator<=>(const sample&) const noexcept =
      default;
  };

  /**
   * @brief Read the current acceleration from the accelerometer.
   *
   * @return boost::leaf::result<sample> - a sample of data or an error
   */
  [[nodiscard]] boost::leaf::result<sample> read() noexcept
  {
    return driver_read();
  }

private:
  virtual boost::leaf::result<sample> driver_read() noexcept = 0;
};
}  // namespace embed
