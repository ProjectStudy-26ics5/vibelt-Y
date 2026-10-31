#ifndef VIBELT_DPY_HPP
#define VIBELT_DPY_HPP

#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

/* ハンドラ */
extern Adafruit_ILI9341 dpy;
extern XPT2046_Touchscreen tch;

/* 初期化 */
void dpy_setup(void);
void gui_setup(void);

#endif
