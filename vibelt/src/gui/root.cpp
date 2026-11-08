#include "../gui.hpp"
#include "../peripherals.hpp"

namespace vb {

#define WEIGHT_BTN_W 60
#define WEIGHT_BTN_H 40
#define WEIGHT_BTN_X (DPY_H_RES - WEIGHT_BTN_W)
#define WEIGHT_BTN_Y 0

#define DEST_BTN_W 60
#define DEST_BTN_H 40
#define DEST_BTN_X (DPY_H_RES - DEST_BTN_W)
#define DEST_BTN_Y (WEIGHT_BTN_Y + WEIGHT_BTN_H + 3)

#define ASTAR_BTN_W 80
#define ASTAR_BTN_H 60
#define ASTAR_BTN_X (DPY_H_RES - ASTAR_BTN_W)
#define ASTAR_BTN_Y (DPY_V_RES - ASTAR_BTN_H)

Result gui_root_setup() {
  dpy.fillScreen(ILI9341_WHITE);

  dpy.setTextColor(ILI9341_BLACK);
  dpy.setTextWrap(false);
  dpy.setTextSize(5);
  dpy.setCursor(10, 10);
  dpy.print("hello");
  dpy.setCursor(10, 80);
  dpy.print(":D");
  dpy.setCursor(10, 140);
  dpy.print("MapView");

  /* 重み設定ボタン */
  dpy.fillRect(
    WEIGHT_BTN_X, WEIGHT_BTN_Y,
    WEIGHT_BTN_W, WEIGHT_BTN_H,
    ILI9341_BLACK
  );
  dpy.setTextColor(ILI9341_WHITE);
  dpy.setCursor(WEIGHT_BTN_X + 20, WEIGHT_BTN_Y);
  dpy.setTextWrap(false);
  dpy.setTextSize(5);
  dpy.print("=");

  /* 目的地設定ボタン */
  dpy.fillRect(
    DEST_BTN_X, DEST_BTN_Y,
    DEST_BTN_W, DEST_BTN_H,
    ILI9341_BLACK
  );
  dpy.setTextColor(ILI9341_WHITE);
  dpy.setCursor(DEST_BTN_X + 20, DEST_BTN_Y + 2);
  dpy.setTextWrap(false);
  dpy.setTextSize(5);
  dpy.print("@");

  /* A*ボタン */
  dpy.fillRect(
    ASTAR_BTN_X, ASTAR_BTN_Y,
    ASTAR_BTN_W, ASTAR_BTN_H,
    ILI9341_BLUE
  );
  dpy.setTextColor(ILI9341_WHITE);
  dpy.setCursor(ASTAR_BTN_X + 10, ASTAR_BTN_Y + 10);
  dpy.setTextWrap(false);
  dpy.setTextSize(5);
  dpy.print("A*");

  return Result::Ok();
}

Result gui_root_runtime() {
  /* 重み設定ボタン */
  if(dpy_touched(
    WEIGHT_BTN_X, WEIGHT_BTN_Y,
    WEIGHT_BTN_W, WEIGHT_BTN_H
  )) {
    return gui_change(GuiKind::WeightConfig);
  }

  return Result::Ok();
}

};
