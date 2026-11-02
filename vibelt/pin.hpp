#ifndef VIBELT_PIN_HPP
#define VIBELT_PIN_HPP

#include "types.hpp"

/**
 * まだ作っていない部分は0にしています
 */

/* オンボードLED */
#define LED_GPIO     38

/* SPI通信 */
#define SPI_SCLK      6
#define SPI_MOSI      5
#define SPI_MISO      4

/* UART通信 */
#define UART_TX       0
#define UART_RX       0

/* ディスプレイ */
#define DPY_SPI_CS   18
#define DPY_DC       16
#define DPY_RST      17
#define DPY_LED_GPIO 15

/* タッチパネル */
#define TCH_SPI_CS    7

/* SDカード */
#define SD_SPI_CS     8
#define SD_SW         0

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
