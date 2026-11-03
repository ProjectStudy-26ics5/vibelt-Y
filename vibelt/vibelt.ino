#include "types.hpp"
#include "pin.hpp"
#include "sd.hpp"
#include "dpy.hpp"

#include <Arduino.h>
#include "src/beltgraph/include/beltgraph.h"

void setup() {
  neopixelWrite(LED_GPIO, 0, 0, 0);

  Serial.begin(115200);
  while(!Serial);
  
  /* 初期化順序は変更してはいけません */
  vibelt_check(pin_setup());

  vibelt_check(sd_setup(0));

  vibelt_check(dpy_setup());
  vibelt_check(gui_neutral_setup());

  /* テスト */
  bgError_t err;
  
  bgAStarVisited_t visited;
  err = bgAStarVisitedCreate(&visited, header.node_count, 0);
  if(err != BG_ERROR_OK) vibelt_panic("");
  
  bgAStarHeap_t heap;
  err = bgAStarHeapCreate(&heap, header.node_count);
  if(err != BG_ERROR_OK) vibelt_panic("");

  bgPath_t path;
  err = bgPathCreate(&path, header.node_count);
  if(err != BG_ERROR_OK) vibelt_panic("");

}

void loop() {
  switch(gui_id) {
    case GUI_NEUTRAL:
      vibelt_check(gui_neutral_runtime());
      break;

    case GUI_WEIGHT:
      vibelt_check(gui_weight_runtime());
      break;

    default:
      vibelt_panic("unknown gui id.");
      break;
  }

  delay(50);
}
