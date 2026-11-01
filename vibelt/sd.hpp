#ifndef VIBELT_SD_HPP
#define VIBELT_SD_HPP

#include <SdFat.h>
#include "src/beltgraph/include/beltgraph/types.h"

/* ハンドラ */
extern SdFat sd;
extern SdCard *card;

/* 初期化 */
void sd_setup(uint32_t starting_sector);

/* 読む */
void sd_read(bgOffset_t pos, void *buffer, bgSize_t size);

#endif
