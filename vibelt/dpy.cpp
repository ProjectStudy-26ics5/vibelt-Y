#include "dpy.hpp"
#include "pin.hpp"
#include "sd.hpp"

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include "src/beltgraph/include/beltgraph.h"

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
  dpy.setCursor(20, DPY_V_RES / 2);
  dpy.setTextWrap(false);
  dpy.setTextSize(5);
  dpy.print("hello! :D");

  /* 重み設定ボタン */
  dpy.fillRect(
    GUI_NEUTRAL_WEIGHT_BTN_X, GUI_NEUTRAL_WEIGHT_BTN_Y,
    GUI_NEUTRAL_WEIGHT_BTN_W, GUI_NEUTRAL_WEIGHT_BTN_H,
    ILI9341_BLACK
  );
  dpy.setTextColor(ILI9341_WHITE);
  dpy.setCursor(GUI_NEUTRAL_WEIGHT_BTN_X + 20, GUI_NEUTRAL_WEIGHT_BTN_Y);
  dpy.setTextWrap(false);
  dpy.setTextSize(5);
  dpy.print("=");

  gui_id = GUI_NEUTRAL;
  return VIBELT_OK;
}

vibelt_result_t gui_neutral_runtime(void) {
  if(tch.touched()) {
    uint16_t px, py;
    tch_point(&px, &py);
    
    /* 重み設定ボタン */
    if(in_rect(
      px, py,
      GUI_NEUTRAL_WEIGHT_BTN_X, GUI_NEUTRAL_WEIGHT_BTN_Y,
      GUI_NEUTRAL_WEIGHT_BTN_W, GUI_NEUTRAL_WEIGHT_BTN_H
    )) {
      gui_weight_setup();
      delay(500);
    }
  }
  
  return VIBELT_OK;
}



#define GUI_WEIGHT_ITEM_W (DPY_H_RES - 20)
#define GUI_WEIGHT_ITEM_H 50
#define GUI_WEIGHT_ITEM_X ((DPY_H_RES - GUI_WEIGHT_ITEM_W) / 2)
#define GUI_WEIGHT_ITEM_Y 10
#define GUI_WEIGHT_ITEM_COUNT (DPY_V_RES / GUI_WEIGHT_ITEM_H)

#define GUI_WEIGHT_ITEM_SLIDER_W 160
#define GUI_WEIGHT_ITEM_SLIDER_H 16
#define GUI_WEIGHT_ITEM_SLIDER_X 0
#define GUI_WEIGHT_ITEM_SLIDER_Y 30

#define GUI_WEIGHT_RETURN_BTN_W 60
#define GUI_WEIGHT_RETURN_BTN_H 40
#define GUI_WEIGHT_RETURN_BTN_X (DPY_H_RES - GUI_WEIGHT_RETURN_BTN_W)
#define GUI_WEIGHT_RETURN_BTN_Y 0

#define GUI_WEIGHT_BACK_BTN_W 60
#define GUI_WEIGHT_BACK_BTN_H 40
#define GUI_WEIGHT_BACK_BTN_X (DPY_H_RES - GUI_WEIGHT_BACK_BTN_W)
#define GUI_WEIGHT_BACK_BTN_Y (DPY_V_RES - GUI_WEIGHT_BACK_BTN_H)

#define GUI_WEIGHT_NEXT_BTN_W 60
#define GUI_WEIGHT_NEXT_BTN_H 40
#define GUI_WEIGHT_NEXT_BTN_X (DPY_H_RES - GUI_WEIGHT_NEXT_BTN_W)
#define GUI_WEIGHT_NEXT_BTN_Y (GUI_WEIGHT_BACK_BTN_Y - GUI_WEIGHT_NEXT_BTN_H - 3)

// 表示する重みの開始番号
static bgIdx_t weight_begin = 0;

static void weight_item(uint8_t i, const char *name) {
  uint16_t x = GUI_WEIGHT_ITEM_X;
  uint16_t y = GUI_WEIGHT_ITEM_Y + GUI_WEIGHT_ITEM_H * i;
  
  /* 名前 */
  dpy.setTextColor(ILI9341_WHITE);
  dpy.setCursor(x, y);
  dpy.setTextWrap(true);

  dpy.setTextSize(2);
  dpy.print(name); // このままでは、右側のボタンと被る可能性があります

  /* スライダ */
  x += GUI_WEIGHT_ITEM_SLIDER_X;
  y += GUI_WEIGHT_ITEM_SLIDER_Y;
  
  dpy.setCursor(x, y);
  dpy.drawFastHLine(
    x, y,
    GUI_WEIGHT_ITEM_SLIDER_W,
    ILI9341_LIGHTGREY
  );
  dpy.drawFastVLine(
    x + GUI_WEIGHT_ITEM_SLIDER_W / 2, y - GUI_WEIGHT_ITEM_SLIDER_H / 2,
    GUI_WEIGHT_ITEM_SLIDER_H,
    ILI9341_LIGHTGREY
  );
}

vibelt_result_t gui_weight_setup(void) {
  dpy.fillScreen(ILI9341_BLACK);

  /* 重み項目 */
  for(uint8_t i = 0; i < GUI_WEIGHT_ITEM_COUNT; i++) {
    bgIdx_t weight_i = weight_begin + i;
    if(weight_i >= nc.dim + wc.dim) break;

    if(weight_i < nc.dim) {
      weight_item(i, bgNodeContextIndicatorName(&nc, weight_i));
    }else {
      weight_item(i, bgWayContextIndicatorName(&wc, weight_i - nc.dim));
    }
  }

  /* 戻るボタン */
  dpy.fillRect(
    GUI_WEIGHT_RETURN_BTN_X, GUI_WEIGHT_RETURN_BTN_Y,
    GUI_WEIGHT_RETURN_BTN_W, GUI_WEIGHT_RETURN_BTN_H,
    ILI9341_GREENYELLOW
  );
  dpy.setTextColor(ILI9341_WHITE);
  dpy.setCursor(GUI_WEIGHT_RETURN_BTN_X + 20, GUI_WEIGHT_RETURN_BTN_Y);
  dpy.setTextWrap(false);
  dpy.setTextSize(5);
  dpy.print("x");

  /* 次の重みに進むボタン */
  if(weight_begin + GUI_WEIGHT_ITEM_COUNT < nc.dim + wc.dim) {
    dpy.fillRect(
      GUI_WEIGHT_NEXT_BTN_X, GUI_WEIGHT_NEXT_BTN_Y,
      GUI_WEIGHT_NEXT_BTN_W, GUI_WEIGHT_NEXT_BTN_H,
      ILI9341_DARKGREY
    );
    dpy.setTextColor(ILI9341_WHITE);
    dpy.setCursor(GUI_WEIGHT_NEXT_BTN_X + 20, GUI_WEIGHT_NEXT_BTN_Y);
    dpy.setTextWrap(false);
    dpy.setTextSize(5);
    dpy.print(">");
  }

  /* 前の重みに戻るボタン */
  if(weight_begin > 0) {
    dpy.fillRect(
      GUI_WEIGHT_BACK_BTN_X, GUI_WEIGHT_BACK_BTN_Y,
      GUI_WEIGHT_BACK_BTN_W, GUI_WEIGHT_BACK_BTN_H,
      ILI9341_DARKGREY
    );
    dpy.setTextColor(ILI9341_WHITE);
    dpy.setCursor(GUI_WEIGHT_BACK_BTN_X + 20, GUI_WEIGHT_BACK_BTN_Y);
    dpy.setTextWrap(false);
    dpy.setTextSize(5);
    dpy.print("<");
  }
  
  gui_id = GUI_WEIGHT;
  return VIBELT_OK;
}

vibelt_result_t gui_weight_runtime(void) {
  if(tch.touched()) {
    uint16_t px, py;
    tch_point(&px, &py);
    
    /* 戻るボタン */
    if(in_rect(
      px, py,
      GUI_WEIGHT_RETURN_BTN_X, GUI_WEIGHT_RETURN_BTN_Y,
      GUI_WEIGHT_RETURN_BTN_W, GUI_WEIGHT_RETURN_BTN_H
    )) {
      gui_neutral_setup();
      delay(500);
    }

    /* 次の重みに進むボタン */
    if(weight_begin + GUI_WEIGHT_ITEM_COUNT < nc.dim + wc.dim && in_rect(
      px, py,
      GUI_WEIGHT_NEXT_BTN_X, GUI_WEIGHT_NEXT_BTN_Y,
      GUI_WEIGHT_NEXT_BTN_W, GUI_WEIGHT_NEXT_BTN_H
    )) {
      weight_begin += GUI_WEIGHT_ITEM_COUNT;
      gui_weight_setup();
      delay(500);
    }

    /* 前の重みに戻るボタン */
    if(weight_begin > 0 && in_rect(
      px, py,
      GUI_WEIGHT_BACK_BTN_X, GUI_WEIGHT_BACK_BTN_Y,
      GUI_WEIGHT_BACK_BTN_W, GUI_WEIGHT_BACK_BTN_H
    )) {
      weight_begin -= GUI_WEIGHT_ITEM_COUNT;
      gui_weight_setup();
      delay(500);
    }
  }

  return VIBELT_OK;
}
