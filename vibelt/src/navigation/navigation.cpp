#include "../navigation.hpp"
#include "../peripherals.hpp"
#include <math.h>

namespace vb {

bool navigating = false;
double dest_lon = NAN;
double dest_lat = NAN;

Result navigation_runtime() {
  if(!navigating) return Result::Ok();

  // テスト
  uint8_t i = (uint8_t)((imu_compass() / 360.0f) * MT_GPIO_NUM);
  mt_low_all();
  mt_high_idx(i);

  return Result::Ok();
}

};
