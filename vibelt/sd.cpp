#include "sd.hpp"
#include "pin.hpp"

#include <driver/sdmmc_host.h>
#include <sdmmc_cmd.h>
#include <esp_err.h>
#include "src/beltgraph/include/beltgraph.h"

#define SECTOR_SIZE 512

static uint8_t sector_buffer[SECTOR_SIZE];
static uint32_t sector_offset = 0;

bgReader_t reader;
bgHeader_t header;
bgNodeContext_t nc;
bgWayContext_t wc;

sdmmc_card_t card;
sdmmc_host_t host;



vibelt_result_t sd_setup(uint32_t starting_sector) {
  sector_offset = starting_sector;

  /* SDカード */
  host = SDMMC_HOST_DEFAULT();
  host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;

  sdmmc_slot_config_t slot = SDMMC_SLOT_CONFIG_DEFAULT();
  slot.width = 4;
  slot.clk = (gpio_num_t)SD_CLK;
  slot.cmd = (gpio_num_t)SD_CMD;
  slot.d0  = (gpio_num_t)SD_DAT0;
  slot.d1  = (gpio_num_t)SD_DAT1;
  slot.d2  = (gpio_num_t)SD_DAT2;
  slot.d3  = (gpio_num_t)SD_DAT3;

  if(host.init() != ESP_OK) {
    return "SD host initialization failed.";
  }
  if(sdmmc_host_init_slot(host.slot, &slot) != ESP_OK) {
    return "SD slot initialization failed.";
  }
  if(sdmmc_card_init(&host, &card) != ESP_OK) {
    return "SD card initialization failed.";
  }

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

  if(sdmmc_read_sectors(&card, sector_buffer, sector, 1) != ESP_OK) {
    return "SD card read failed.";
  }
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
