#include "pin.hpp"
#include "sd.hpp"
#include "dpy.hpp"

void setup() {
  /* 初期化順序は変更してはいけません */
  pin_setup();

  sd_setup(0);
  
  dpy_setup();
  gui_setup();

  /* テスト */
  Serial.begin(115200);
  while(!Serial);

  uint8_t buffer[6];
  sd_read(0, buffer, 6);
  for(int i = 0; i < 6; i++) {
    Serial.printf("%02x ", buffer[i]);
  }
}

void loop() {
}
