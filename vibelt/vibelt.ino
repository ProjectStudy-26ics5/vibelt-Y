#include "pin.hpp"
#include "sd.hpp"
#include "dpy.hpp"

#include "src/beltgraph/include/beltgraph.h"

void setup() {
  /* 初期化順序は変更してはいけません */
  pin_setup();

  sd_setup(0);
  
  dpy_setup();
  gui_setup();

  /* テスト */
  Serial.begin(115200);
  while(!Serial);

  Serial.printf("nc_pos = %llu\n", header.nc_pos);
  Serial.printf("nc_dim = %u\n", header.nc_dim);
  Serial.printf("nc_ind_name_size = %llu\n", header.nc_ind_name_size);
  Serial.printf("nc_ind_desc_size = %llu\n", header.nc_ind_desc_size);

  Serial.printf("wc_pos = %llu\n", header.wc_pos);
  Serial.printf("wc_dim = %u\n", header.wc_dim);
  Serial.printf("wc_ind_name_size = %llu\n", header.wc_ind_name_size);
  Serial.printf("wc_ind_desc_size = %llu\n", header.wc_ind_desc_size);
  
  Serial.printf("node_pos = %llu\n", header.node_pos);
  Serial.printf("node_count = %u\n", header.node_count);
  Serial.printf("node_stride = %llu\n", header.node_stride);

  Serial.printf("way_pos = %llu\n", header.way_pos);
  Serial.printf("way_count = %u\n", header.way_count);
  Serial.printf("way_stride = %llu\n", header.way_stride);
}

void loop() {
}
