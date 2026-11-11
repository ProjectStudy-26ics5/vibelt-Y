#include "../peripherals.hpp"

namespace vb {

static const uint8_t MT_GPIO_ARRAY[MT_GPIO_NUM] = {
  MT0_GPIO,
  MT1_GPIO,
  MT2_GPIO,
  MT3_GPIO,
  MT4_GPIO,
  MT5_GPIO,
  MT6_GPIO,
  MT7_GPIO
};

Result mt_setup() {
  for(uint8_t i = 0; i < MT_GPIO_NUM; i++) {
    pinMode(MT_GPIO_ARRAY[i], OUTPUT);
    digitalWrite(MT_GPIO_ARRAY[i], LOW);
  }

  return Result::Ok();
}

void mt_low_all() {
  for(uint8_t i = 0; i < MT_GPIO_NUM; i++) {
    digitalWrite(MT_GPIO_ARRAY[i], LOW);
  }
}

void mt_high_idx(uint8_t i) {
  digitalWrite(MT_GPIO_ARRAY[i], HIGH);
}

};
