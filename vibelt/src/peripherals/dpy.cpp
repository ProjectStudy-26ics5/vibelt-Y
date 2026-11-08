#include "../peripherals.hpp"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

namespace vb {

/**
 * ディスプレイのダブルバッファリングは
 * メモリ消費と、描画が重要な要素ではないことから
 * しない方針でいきます
 */
Adafruit_ILI9341 dpy(DPY_SPI_CS, DPY_DC, DPY_RST);
static XPT2046_Touchscreen tch(TCH_SPI_CS);
uint16_t tch_x = UINT16_MAX;
uint16_t tch_y = UINT16_MAX;

Result dpy_setup() {
  // SPI通信
  if(!SPI.begin(DPY_SPI_CLK, DPY_SPI_MISO, DPY_SPI_MOSI)) {
    return Result::Err("Display SPI initialization failed.");
  }

  // バックライト
  pinMode(DPY_LED_GPIO, OUTPUT);
  digitalWrite(DPY_LED_GPIO, HIGH);

  // ディスプレイハンドラ
  dpy.begin();
  dpy.setRotation(1);

  // タッチセンサハンドラ
  tch.begin();
  tch.setRotation(3);

  return Result::Ok();
}



Result dpy_runtime() {
  if(tch.touched()) {
    TS_Point p = tch.getPoint();

    tch_x = (uint16_t)((float)p.x / 4096.0f * (float)DPY_H_RES);
    tch_y = (uint16_t)((float)p.y / 4096.0f * (float)DPY_V_RES);
  }else {

    tch_x = UINT16_MAX;
    tch_y = UINT16_MAX;
  }

  return Result::Ok();
}

bool dpy_touched(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  if(x <= tch_x && tch_x < x + w && y <= tch_y && tch_y < y + h) {
    return true;
  }
  return false;
}

};
