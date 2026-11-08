#include "../gui.hpp"
#include "../peripherals.hpp"
#include "../beltgraph/include/beltgraph.h"

namespace vb {

static bgIdx_t start_weight = 0;

#define ITEM_W DPY_H_RES
#define ITEM_H 50
#define ITEM_X 10
#define ITEM_Y 10
#define ITEM_COUNT (DPY_V_RES / ITEM_H)

#define ITEM_SLIDER_W 161
#define ITEM_SLIDER_H 25
#define ITEM_SLIDER_X 20
#define ITEM_SLIDER_Y 18
#define ITEM_SLIDER_HW (ITEM_SLIDER_W / 2)
#define ITEM_SLIDER_HH (ITEM_SLIDER_H / 2)

#define RETURN_BTN_W 60
#define RETURN_BTN_H 40
#define RETURN_BTN_X (DPY_H_RES - RETURN_BTN_W)
#define RETURN_BTN_Y 0

#define BACK_BTN_W 60
#define BACK_BTN_H 40
#define BACK_BTN_X (DPY_H_RES - BACK_BTN_W)
#define BACK_BTN_Y (DPY_V_RES - BACK_BTN_H)

#define NEXT_BTN_W 60
#define NEXT_BTN_H 40
#define NEXT_BTN_X (DPY_H_RES - NEXT_BTN_W)
#define NEXT_BTN_Y (BACK_BTN_Y - NEXT_BTN_H - 3)

static void weight_name(uint16_t x, uint16_t y, const char *name) {
  dpy.setTextColor(ILI9341_WHITE);
  dpy.setCursor(x, y);
  dpy.setTextWrap(true);
  dpy.setTextSize(2);
  dpy.print(name); // このままでは、右側のボタンと被る可能性があります
}

static void weight_slider(uint16_t x, uint16_t y, bgWeight_t weight) {
  dpy.fillRect(
    x, y,
    ITEM_SLIDER_W, ITEM_SLIDER_H,
    ILI9341_BLACK
  );
  dpy.drawFastHLine(
    x, y + ITEM_SLIDER_HH,
    ITEM_SLIDER_W,
    ILI9341_LIGHTGREY
  );
  dpy.drawFastVLine(
    x + ITEM_SLIDER_HW, y,
    ITEM_SLIDER_H,
    ILI9341_LIGHTGREY
  );

  if(weight > 0) {
    dpy.fillRect(
      x + ITEM_SLIDER_HW + 1, y + ITEM_SLIDER_HH - 1,
      (uint16_t)(weight * ITEM_SLIDER_HW), 3,
      ILI9341_PINK
    );
  }else if(weight < 0) {
    dpy.fillRect(
      x + ITEM_SLIDER_HW - 1, y + ITEM_SLIDER_HH - 1,
      (uint16_t)(weight * ITEM_SLIDER_HW), 3,
      ILI9341_CYAN
    );
  }
}

Result gui_weight_config_setup() {
  dpy.fillScreen(ILI9341_BLACK);

  /* 重み項目 */
  for(uint8_t i = 0; i < ITEM_COUNT; i++) {
    bgIdx_t weight_i = start_weight + i;
    if(weight_i >= nc.dim + wc.dim) break;

    uint16_t x = ITEM_X;
    uint16_t y = ITEM_Y + ITEM_H * i;

    if(weight_i < nc.dim) {
      weight_name(x, y, bgNodeContextIndicatorName(&nc, weight_i));
      weight_slider(x + ITEM_SLIDER_X, y + ITEM_SLIDER_Y, nc.weights[weight_i]);
    }else {
      weight_i -= nc.dim;

      weight_name(x, y, bgWayContextIndicatorName(&wc, weight_i));
      weight_slider(x + ITEM_SLIDER_X, y + ITEM_SLIDER_Y, wc.weights[weight_i]);
    }
  }

  /* 戻るボタン */
  dpy.fillRect(
    RETURN_BTN_X, RETURN_BTN_Y,
    RETURN_BTN_W, RETURN_BTN_H,
    ILI9341_GREENYELLOW
  );
  dpy.setTextColor(ILI9341_WHITE);
  dpy.setCursor(RETURN_BTN_X + 20, RETURN_BTN_Y);
  dpy.setTextWrap(false);
  dpy.setTextSize(5);
  dpy.print("x");

  /* 次の重みに進むボタン */
  if(start_weight + ITEM_COUNT < nc.dim + wc.dim) {
    dpy.fillRect(
      NEXT_BTN_X, NEXT_BTN_Y,
      NEXT_BTN_W, NEXT_BTN_H,
      ILI9341_DARKGREY
    );
    dpy.setTextColor(ILI9341_WHITE);
    dpy.setCursor(NEXT_BTN_X + 20, NEXT_BTN_Y + 2);
    dpy.setTextWrap(false);
    dpy.setTextSize(5);
    dpy.print(">");
  }

  /* 前の重みに戻るボタン */
  if(start_weight > 0) {
    dpy.fillRect(
      BACK_BTN_X, BACK_BTN_Y,
      BACK_BTN_W, BACK_BTN_H,
      ILI9341_DARKGREY
    );
    dpy.setTextColor(ILI9341_WHITE);
    dpy.setCursor(BACK_BTN_X + 20, BACK_BTN_Y + 2);
    dpy.setTextWrap(false);
    dpy.setTextSize(5);
    dpy.print("<");
  }

  return Result::Ok();
}

Result gui_weight_config_runtime() {
  /* 戻るボタン */
  if(dpy_touched(
    RETURN_BTN_X, RETURN_BTN_Y,
    RETURN_BTN_W, RETURN_BTN_H
  )) {
    start_weight = 0;
    return gui_change(GuiKind::Root);
  }

  /* 次の重みに進むボタン */
  if(start_weight + ITEM_COUNT < nc.dim + wc.dim && dpy_touched(
    NEXT_BTN_X, NEXT_BTN_Y,
    NEXT_BTN_W, NEXT_BTN_H
  )) {
    start_weight += ITEM_COUNT;
    return gui_change(GuiKind::WeightConfig);
  }

  /* 前の重みに戻るボタン */
  if(start_weight > 0 && dpy_touched(
    BACK_BTN_X, BACK_BTN_Y,
    BACK_BTN_W, BACK_BTN_H
  )) {
    start_weight -= ITEM_COUNT;
    return gui_change(GuiKind::WeightConfig);
  }

  /* スライダ */
  for(uint8_t i = 0; i < ITEM_COUNT; i++) {
    bgIdx_t weight_i = start_weight + i;
    if(weight_i >= nc.dim + wc.dim) break;

    uint16_t x = ITEM_X + ITEM_SLIDER_X;
    uint16_t y = ITEM_Y + ITEM_H * i + ITEM_SLIDER_Y;

    if(dpy_touched(
      x, y,
      ITEM_SLIDER_W, ITEM_SLIDER_H
    )) {
      bgWeight_t weight = ((float)tch_x - (float)(x + ITEM_SLIDER_HW)) / (float)(ITEM_SLIDER_HW);

      if(weight_i < nc.dim) {
        weight_slider(x, y, (nc.weights[weight_i] = weight));
      }else {
        weight_i -= nc.dim;

        weight_slider(x, y, (wc.weights[weight_i] = weight));
      }

      return Result::Ok();
    }
  }

  return Result::Ok();
}

};
