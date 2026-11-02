#include "pin.hpp"

#include <Arduino.h>
#include <SPI.h>

const uint8_t MT_GPIO_ARRAY[MT_GPIO_NUM] = {
  MT0_GPIO,
  MT1_GPIO,
  MT2_GPIO,
  MT3_GPIO,
  MT4_GPIO,
  MT5_GPIO,
  MT6_GPIO,
  MT7_GPIO
};

vibelt_result_t pin_setup(void) {
  if(!SPI.begin(SPI_SCLK, SPI_MISO, SPI_MOSI)) {
    return "SPI initialization failed.";
  }

  pinMode(DPY_LED_GPIO, OUTPUT);

  return VIBELT_OK;
}
