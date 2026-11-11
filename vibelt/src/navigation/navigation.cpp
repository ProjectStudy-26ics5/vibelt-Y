#include "../navigation.hpp"
#include "../peripherals.hpp"

namespace vb {

bool navigating = false;

Result navigation_runtime() {
  uint8_t i = (uint8_t)((imu_compass() / 360.0f) * MT_GPIO_NUM);
  mt_low_all();
  mt_high_idx(i);

  return Result::Ok();
}

};
