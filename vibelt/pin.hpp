#ifndef VIBELT_PIN_HPP
#define VIBELT_PIN_HPP

#include "types.hpp"

/**
 * まだ作っていない部分は0にしています
 */

/* オンボードLED */
#define LED_GPIO     38

/* SDカード */
#define SD_CMD       12
#define SD_CLK       11
#define SD_DAT0      10
#define SD_DAT1       9
#define SD_DAT2      14
#define SD_DAT3      13
#define SD_SW        46

/* SPI通信 */
#define SPI_CLK       7
#define SPI_MOSI      5
#define SPI_MISO      4

/* ディスプレイ */
#define DPY_SPI_CS   18
#define DPY_DC       16
#define DPY_RST      17
#define DPY_LED_GPIO 15

/* タッチパネル */
#define TCH_SPI_CS    6

/* 振動モータ */
#define MT0_GPIO      0
#define MT1_GPIO      0
#define MT2_GPIO      0
#define MT3_GPIO      0
#define MT4_GPIO      0
#define MT5_GPIO      0
#define MT6_GPIO      0
#define MT7_GPIO      0

#define MT_GPIO_NUM 8
extern const uint8_t MT_GPIO_ARRAY[MT_GPIO_NUM];

/* 初期化 */
vibelt_result_t pin_setup(void);

#endif
