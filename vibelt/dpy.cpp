#include "dpy.hpp"
#include "pin.hpp"

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>

Adafruit_ILI9341 dpy(DPY_SPI_CS, DPY_DC, DPY_RST);
XPT2046_Touchscreen tch(TCH_SPI_CS);

void dpy_setup(void) {
  digitalWrite(DPY_LED_GPIO, HIGH);

  dpy.begin();
  dpy.setRotation(1);
  
  tch.begin();
  tch.setRotation(1);
}

void gui_setup(void) {
  dpy.fillScreen(ILI9341_BLACK);

  dpy.setTextColor(ILI9341_WHITE);
  dpy.setTextSize(5);
  dpy.setCursor(20, 80);
  dpy.println("hello");
}
