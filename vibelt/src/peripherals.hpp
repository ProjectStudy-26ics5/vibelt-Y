#ifndef VIBELT_PERIPHERALS_HPP
#define VIBELT_PERIPHERALS_HPP

#include "common.hpp"
#include <Arduino.h>
#include <Adafruit_ILI9341.h>

namespace vb {

/* ピンアサイン */
// オンボードLED
#define LED_GPIO     38

// SDカード
#define SD_CMD       12
#define SD_CLK       11
#define SD_DAT0      10
#define SD_DAT1       9
#define SD_DAT2      14
#define SD_DAT3      13
#define SD_SW        46

// ディスプレイ
#define DPY_SPI_CLK   7
#define DPY_SPI_MOSI  5
#define DPY_SPI_MISO  4
#define DPY_SPI_CS   18
#define DPY_DC       16
#define DPY_RST      17
#define DPY_LED_GPIO 15

#define TCH_SPI_CS    6

// 振動モータ
#define MT0_GPIO      0
#define MT1_GPIO      0
#define MT2_GPIO      0
#define MT3_GPIO      0
#define MT4_GPIO      0
#define MT5_GPIO      0
#define MT6_GPIO      0
#define MT7_GPIO      0



/* 抽象制御 */
// SDカード
Result sd_setup(size_t start_sector);
void sd_read(bgOffset_t pos, void *buffer, bgSize_t size);

// ディスプレイ
#define DPY_H_RES 320
#define DPY_V_RES 240

extern Adafruit_ILI9341 dpy;
extern uint16_t tch_x;
extern uint16_t tch_y;

Result dpy_setup();
Result dpy_runtime();
bool dpy_touched(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

};

#endif
