#include "types.hpp"
#include "pin.hpp"
#include "sd.hpp"
#include "dpy.hpp"

#include <Arduino.h>
#include <time.h>
#include "src/beltgraph/include/beltgraph.h"

void setup() {
  neopixelWrite(LED_GPIO, 0, 0, 0);

  Serial.begin(115200);
  while(!Serial);
  
  /* 初期化順序は変更してはいけません */
  vibelt_check(pin_setup());

  vibelt_check(sd_setup(0));

  vibelt_check(dpy_setup());
  vibelt_check(gui_neutral_setup());

  /* テスト */
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

  /* 経路探索テスト 計算が大変なので、普段はしない */
  return;
  bgError_t err;

  bgNode_t node;
  err = bgNodeCreate(&node, &header);
  if(err != BG_ERROR_OK) vibelt_panic("");

  bgWay_t way;
  err = bgWayCreate(&way, &header);
  if(err != BG_ERROR_OK) vibelt_panic("");
  
  bgAStarVisited_t visited;
  err = bgAStarVisitedCreate(&visited, header.node_count, 0);
  if(err != BG_ERROR_OK) vibelt_panic("");
  
  bgAStarHeap_t heap;
  err = bgAStarHeapCreate(&heap, header.node_count);
  if(err != BG_ERROR_OK) vibelt_panic("");

  bgPath_t path;
  err = bgPathCreate(&path, header.node_count);
  if(err != BG_ERROR_OK) vibelt_panic("");

  bgNodeFetchNearest(&node, &reader, &header, 139.751, 35.635);
  bgIdx_t start_node_idx = node.idx;
  bgNodeFetchNearest(&node, &reader, &header, 139.800, 35.635);
  bgIdx_t goal_node_idx = node.idx;

  clock_t start = clock();
  err = bgAStar(
    &reader, &header,
    &nc, &wc,
    &node, &way,
    &visited, &heap,
    start_node_idx, goal_node_idx,
    2,
    &path
  );
  clock_t end = clock();

  if(err == BG_ERROR_OK) {
    Serial.printf("Ok !\n");

    Serial.printf("path = {");
    for(bgIdx_t i = 0; i < path.size; i++) {
      Serial.printf("%u -> ", path.node_idx_order[i]);
    }
    Serial.printf("\b\b\b\b}    \n\n");
  }else {
    Serial.printf("Err !\n");
  }
  Serial.printf("time: %.4lf sec\n", (double)(end - start) / CLOCKS_PER_SEC);
  Serial.printf("| visited | allocated: %u, used: %u\n", visited.capacity, visited.size);
  Serial.printf("| heap    | allocated: %u, used: %u\n", heap.capacity, heap.peak_size);
  Serial.printf("| path    | allocated: %u, used: %u\n", path.capacity, path.size);
}

void loop() {
  switch(gui_id) {
    case GUI_NEUTRAL:
      vibelt_check(gui_neutral_runtime());
      break;

    case GUI_WEIGHT:
      vibelt_check(gui_weight_runtime());
      break;

    default:
      vibelt_panic("unknown gui id.");
      break;
  }

  delay(50);
}
