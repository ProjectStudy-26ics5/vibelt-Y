#include "sd.hpp"
#include "pin.hpp"

#include <SPI.h>
#include <SdFat.h>
#include "src/beltgraph/include/beltgraph.h"

#define SECTOR_SIZE 512

static uint8_t sector_buffer[SECTOR_SIZE];
static uint32_t sector_offset = 0;

static SdFat sd;
static SdCard *card = NULL;

bgReader_t reader;
bgHeader_t header;
bgNodeContext_t nc;
bgWayContext_t wc;



vibelt_result_t sd_setup(uint32_t starting_sector) {
  /* SDカード */
  SdSpiConfig cfg(SD_SPI_CS, SHARED_SPI, SD_SCK_MHZ(20));
  if(!sd.cardBegin(cfg)) {
    return "SD card initialization failed.";
  }
  card = sd.card();
  sector_offset = starting_sector;

  /* bgデータ */
  bgError_t err;
  err = bgReaderCreate(&reader, sd_read);
  if(err != BG_ERROR_OK) goto bg_error;

  err = bgHeaderCreate(&header);
  if(err != BG_ERROR_OK) goto bg_error;
  bgHeaderFetch(&header, &reader);

  err = bgNodeContextCreate(&nc, &header);
  if(err != BG_ERROR_OK) goto bg_error;
  bgNodeContextFetch(&nc, &reader, &header);

  err = bgWayContextCreate(&wc, &header);
  if(err != BG_ERROR_OK) goto bg_error;
  bgWayContextFetch(&wc, &reader, &header);

  for(bgIdx_t i = 0; i < nc.dim; i++) {
    bgNodeContextSetWeight(&nc, i, 0);
  }
  for(bgIdx_t i = 0; i < wc.dim; i++) {
    bgWayContextSetWeight(&wc, i, 0);
  }

  return VIBELT_OK;

bg_error:
  return "bg initialization failed.";
}



/**
 * sector_offset + sectorの物理セクタを読みます
 */
static vibelt_result_t fetch_sector(uint32_t sector) {
  sector += sector_offset;
  static uint32_t last_fetch_sector = UINT32_MAX;

  if(last_fetch_sector == sector) return VIBELT_OK;

  card->readSector(sector, sector_buffer);
  last_fetch_sector = sector;

  return VIBELT_OK;
}

void sd_read(bgOffset_t pos, void *buffer, bgSize_t size) {
  vibelt_result_t result;
  uint32_t sector, lane;

  for(bgSize_t i = 0; i < size; i++) {
    sector = (pos + i) / SECTOR_SIZE;
    lane   = (pos + i) % SECTOR_SIZE;

    // 現状のbgの実装上、読み取り失敗の選択肢はがないので、パニックします。
    vibelt_check(fetch_sector(sector));

    ((uint8_t *)buffer)[i] = sector_buffer[lane];
  }
}
