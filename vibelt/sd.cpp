#include "sd.hpp"
#include "pin.hpp"

#include <SPI.h>
#include <SdFat.h>
#include "src/beltgraph/include/beltgraph/types.h"

#define SECTOR_SIZE 512

static uint8_t sector_buffer[SECTOR_SIZE];
static uint32_t sector_offset = 0;

SdFat sd;
SdCard *card = NULL;

void sd_setup(uint32_t starting_sector) {
  /**
   * sd.beginとsd.cardにエラーチェックをしたほうがいいです。
   * 今は、エラー時の選択肢がないのでほっときます。
   */
  sd.begin(SD_SPI_CS, SD_SCK_MHZ(20));
  card = sd.card();
  sector_offset = starting_sector;
}



/**
 * sector_offset + sectorの物理セクタを読みます
 */
static void fetch_sector(uint32_t sector) {
  sector += sector_offset;
  static uint32_t last_fetch_sector = UINT32_MAX;

  /**
   * readSectorにエラーチェックをしたほうがいいです。
   * 今は、エラー時の選択肢がないのでほっときます。
   */
  if(last_fetch_sector != sector) {
    card->readSector(sector, sector_buffer);
    last_fetch_sector = sector;
  }
}

void sd_read(bgOffset_t pos, void *buffer, bgSize_t size) {
  for(bgSize_t i = 0; i < size; i++) {
    uint32_t sector = pos / SECTOR_SIZE;
    uint16_t lane = pos % SECTOR_SIZE;

    fetch_sector(sector);
    ((uint8_t *)buffer)[i] = sector_buffer[lane];
  }
}
