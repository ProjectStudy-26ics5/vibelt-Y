#include "src/common.hpp"
#include "src/peripherals.hpp"
#include "src/gui.hpp"
#include "src/navigation.hpp"

#include <Arduino.h>
#include <time.h>
#include "src/beltgraph/include/beltgraph.h"

void setup() {
  neopixelWrite(LED_GPIO, 0, 0, 0);
  Serial.begin(115200);

  vb::check(vb::mt_setup());
  vb::check(vb::sd_setup(0));
  vb::check(vb::bg_setup());
  vb::check(vb::imu_setup());
  vb::check(vb::gps_setup());
  vb::check(vb::dpy_setup());
  vb::check(vb::gui_change(vb::GuiKind::Root));

  /* テスト */
  Serial.printf("nc_pos = %llu\n", vb::header.nc_pos);
  Serial.printf("nc_dim = %u\n", vb::header.nc_dim);
  Serial.printf("nc_ind_name_size = %llu\n", vb::header.nc_ind_name_size);
  Serial.printf("nc_ind_desc_size = %llu\n", vb::header.nc_ind_desc_size);

  Serial.printf("wc_pos = %llu\n", vb::header.wc_pos);
  Serial.printf("wc_dim = %u\n", vb::header.wc_dim);
  Serial.printf("wc_ind_name_size = %llu\n", vb::header.wc_ind_name_size);
  Serial.printf("wc_ind_desc_size = %llu\n", vb::header.wc_ind_desc_size);

  Serial.printf("node_pos = %llu\n", vb::header.node_pos);
  Serial.printf("node_count = %u\n", vb::header.node_count);
  Serial.printf("node_stride = %llu\n", vb::header.node_stride);

  Serial.printf("way_pos = %llu\n", vb::header.way_pos);
  Serial.printf("way_count = %u\n", vb::header.way_count);
  Serial.printf("way_stride = %llu\n", vb::header.way_stride);

  /* 経路探索テスト */
  return;
  bgNodeFetchNearest(&vb::node, &vb::reader, &vb::header, 139.751, 35.635);
  bgIdx_t start_node_idx = vb::node.idx;
  bgNodeFetchNearest(&vb::node, &vb::reader, &vb::header, 139.800, 35.635);
  bgIdx_t goal_node_idx = vb::node.idx;
  
  clock_t start = clock();
  vb::check(vb::bg_astar(start_node_idx, goal_node_idx, 2));
  clock_t end = clock();

  Serial.printf("time: %.4lf sec\n", (double)(end - start) / CLOCKS_PER_SEC);
  Serial.printf("| visited | allocated: %u, used: %u\n", vb::visited.capacity, vb::visited.size);
  Serial.printf("| heap    | allocated: %u, used: %u\n", vb::heap.capacity, vb::heap.peak_size);
  Serial.printf("| path    | allocated: %u, used: %u\n", vb::path.capacity, vb::path.size);
}

void loop() {
  vb::check(vb::imu_runtime());
  vb::check(vb::gps_runtime());
  vb::check(vb::dpy_runtime());
  vb::check(vb::gui_runtime());
  vb::check(vb::navigation_runtime());

  // Serial.printf("compass = %f\n", vb::imu_compass());
  // Serial.printf("lon = %lf, lat = %lf\n", vb::gps_lon, vb::gps_lat);

  // static uint8_t i = 0;
  // i = (i + 1) % MT_GPIO_NUM;
  // vb::mt_low_all();
  // vb::mt_high_idx(i);

  delay(50);
}
