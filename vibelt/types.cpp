#include "types.hpp"
#include "pin.hpp"

#include <Arduino.h>

void vibelt_panic(vibelt_result_t err) {
  Serial.printf("PANIC: %s\n", err);

  for(;;) {
    neopixelWrite(LED_GPIO, 50, 0, 0);
    delay(500);
    neopixelWrite(LED_GPIO, 0, 0, 0);
    delay(500);
  }
}

void vibelt_check(vibelt_result_t err) {
  if(err != VIBELT_OK) {
    vibelt_panic(err);
  }
}
