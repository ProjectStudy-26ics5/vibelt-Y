#include "../peripherals.hpp"
#include <driver/sdmmc_host.h>
#include <sdmmc_cmd.h>
#include <esp_err.h>

namespace vb {

#define SECTOR_SIZE 512

static uint8_t sector_buffer[SECTOR_SIZE];
static size_t sector_offset = 0;
static sdmmc_card_t card;
static sdmmc_host_t host;

Result sd_setup(size_t start_sector) {
  sector_offset = start_sector;

  // 挿入スイッチ
  pinMode(SD_SW, INPUT);
  if(digitalRead(SD_SW) == LOW) {
    return Result::Err("No SD card is inserted.");
  }

  // SDカードハンドラ
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
    return Result::Err("SD host initialization failed.");
  }
  if(sdmmc_host_init_slot(host.slot, &slot) != ESP_OK) {
    return Result::Err("SD slot initialization failed.");
  }
  if(sdmmc_card_init(&host, &card) != ESP_OK) {
    return Result::Err("SD card initialization failed.");
  }

  return Result::Ok();
}



static Result fetch_sector(size_t sector) {
  sector += sector_offset;
  static size_t last_fetch_sector = SIZE_MAX;

  if(last_fetch_sector == sector) {
    return Result::Ok();
  }

  if(sdmmc_read_sectors(&card, sector_buffer, sector, 1) != ESP_OK) {
    return Result::Err("SD card read failed.");
  }
  last_fetch_sector = sector;

  return Result::Ok();
}

void sd_read(bgOffset_t pos, void *buffer, bgSize_t size) {
  size_t sector, lane;

  for(bgSize_t i = 0; i < size; i++) {
    sector = (pos + i) / SECTOR_SIZE;
    lane   = (pos + i) % SECTOR_SIZE;

    // 現状のbgの実装上、読み取り失敗の選択肢はがないので、パニックします。
    check(fetch_sector(sector));

    ((uint8_t *)buffer)[i] = sector_buffer[lane];
  }
}

};
