#ifndef VIBELT_PERIPHERALS_HPP
#define VIBELT_PERIPHERALS_HPP

#include "common.hpp"
#include <Arduino.h>
#include <Adafruit_ILI9341.h>

namespace vb {

/* ピンアサイン */

/**
 * [注意するGPIO]
 * 0, 3, 45, 46 は起動時のレベルで、起動構成が変わる
 * 19, 20 はUSBを使はないので利用できます
 * 43, 44 は書き込みやシリアル通信時に接続しなければ使えます
 * 26 - 32 はFLash接続用で使えません (DevKitでは露出していません)
 * 33 - 37 はPSRAM接続用で使えません
 */

// オンボードLED
#define LED_GPIO     38

// SDカード
#define SD_CMD       12
#define SD_CLK       11
#define SD_DAT0      10
#define SD_DAT1       9
#define SD_DAT2      14
#define SD_DAT3      13
#define SD_SW         8

// ディスプレイ
#define DPY_SPI_CLK  47
#define DPY_SPI_MOSI 20
#define DPY_SPI_MISO 19
#define DPY_SPI_CS   40
#define DPY_DC       38
#define DPY_RST      39
#define DPY_LED_GPIO 48

#define TCH_SPI_CS   21

// 慣性計測ユニット
#define IMU_I2C_SDA  18
#define IMU_I2C_SCL  17

// GPS (モジュールのRXをマイコンのTXに、TXをRXに接続することに注意してください)
#define GPS_UART_TX  16
#define GPS_UART_RX  15

// 振動モータ
#define MT0_GPIO      7
#define MT1_GPIO      6
#define MT2_GPIO      5
#define MT3_GPIO      4
#define MT4_GPIO      1
#define MT5_GPIO      2
#define MT6_GPIO     42
#define MT7_GPIO     41



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

// 慣性計測ユニット
Result imu_setup();
Result imu_runtime();
float imu_compass();

// GPS
extern double gps_lon;
extern double gps_lat;

Result gps_setup();
Result gps_runtime();

// 振動モータ
#define MT_GPIO_NUM 8

Result mt_setup();
void mt_low_all();
void mt_high_idx(uint8_t i);

};

#endif
