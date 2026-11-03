#include "dpy.hpp"
#include "pin.hpp"

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

/**
 * ディスプレイのダブルバッファリングは
 * メモリ消費と、描画が重要な要素ではないことから
 * しない方針でいきます
 */
static Adafruit_ILI9341 dpy(DPY_SPI_CS, DPY_DC, DPY_RST);
static XPT2046_Touchscreen tch(TCH_SPI_CS);

gui_kind_t gui_id = GUI_NEUTRAL;

vibelt_result_t dpy_setup(void) {
  digitalWrite(DPY_LED_GPIO, HIGH);

  dpy.begin();
  dpy.setRotation(1);
  
  tch.begin();
  tch.setRotation(3);

  return VIBELT_OK;
}



static void tch_point(uint16_t *px, uint16_t *py) {
  TS_Point p = tch.getPoint();

  *px = (float)p.x / 4095 * DPY_H_RES;
  *py = (float)p.y / 4095 * DPY_V_RES;
}

static bool in_rect(uint16_t px, uint16_t py, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  if(x <= px && px <= x + w && y <= py && py <= y + h) {
    return true;
  }
  return false;
}



#define GUI_NEUTRAL_WEIGHT_BTN_W 60
#define GUI_NEUTRAL_WEIGHT_BTN_H 40
#define GUI_NEUTRAL_WEIGHT_BTN_X (DPY_H_RES - GUI_NEUTRAL_WEIGHT_BTN_W)
#define GUI_NEUTRAL_WEIGHT_BTN_Y 0

vibelt_result_t gui_neutral_setup(void) {
  dpy.fillScreen(ILI9341_WHITE);

  dpy.setTextColor(ILI9341_BLACK);
  dpy.setTextSize(6);
  dpy.setTextWrap(false);
  dpy.setCursor(20, DPY_V_RES / 2);
  dpy.print("hello");

  // 重み設定ボタン
  dpy.fillRect(
    GUI_NEUTRAL_WEIGHT_BTN_X, GUI_NEUTRAL_WEIGHT_BTN_Y,
    GUI_NEUTRAL_WEIGHT_BTN_W, GUI_NEUTRAL_WEIGHT_BTN_H,
    ILI9341_BLACK
  );
  dpy.setTextColor(ILI9341_WHITE);
  dpy.setTextSize(5);
  dpy.setTextWrap(false);
  dpy.setCursor(GUI_NEUTRAL_WEIGHT_BTN_X + 20, GUI_NEUTRAL_WEIGHT_BTN_Y);
  dpy.print("=");

  gui_id = GUI_NEUTRAL;
  return VIBELT_OK;
}

vibelt_result_t gui_neutral_runtime(void) {
  if(tch.touched()) {
    uint16_t px, py;
    tch_point(&px, &py);
    
    if(in_rect(
      px, py,
      GUI_NEUTRAL_WEIGHT_BTN_X, GUI_NEUTRAL_WEIGHT_BTN_Y,
      GUI_NEUTRAL_WEIGHT_BTN_W, GUI_NEUTRAL_WEIGHT_BTN_H
    )) {
      Serial.println("test");
    }
  }
  
  return VIBELT_OK;
}
