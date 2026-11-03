#ifndef VIBELT_SD_HPP
#define VIBELT_SD_HPP

#include "types.hpp"

/* bg */
extern bgReader_t reader;
extern bgHeader_t header;
extern bgNodeContext_t nc;
extern bgWayContext_t wc;

/* 初期化 */
vibelt_result_t sd_setup(uint32_t starting_sector);

/* 読む */
void sd_read(bgOffset_t pos, void *buffer, bgSize_t size);

#endif
