#include "../common.hpp"
#include "../peripherals.hpp"
#include "../beltgraph/include/beltgraph.h"

namespace vb {

bgReader_t reader;
bgHeader_t header;
bgNodeContext_t nc;
bgWayContext_t wc;
bgNode_t node;
bgWay_t way;
bgAStarVisited_t visited;
bgAStarHeap_t heap;
bgPath_t path;

Result bg_setup() {
  if(bgReaderCreate(&reader, sd_read) != BG_ERROR_OK) goto err;
  if(bgHeaderCreate(&header) != BG_ERROR_OK) goto err;
  bgHeaderFetch(&header, &reader);

  if(bgNodeContextCreate(&nc, &header) != BG_ERROR_OK) goto err;
  bgNodeContextFetch(&nc, &reader, &header);
  if(bgWayContextCreate(&wc, &header) != BG_ERROR_OK) goto err;
  bgWayContextFetch(&wc, &reader, &header);

  if(bgNodeCreate(&node, &header) != BG_ERROR_OK) goto err;
  if(bgWayCreate(&way, &header) != BG_ERROR_OK) goto err;

  // 今はノード数すべてのリソースを確保していますが、今後変更します
  if(bgAStarVisitedCreate(&visited, header.node_count, 0) != BG_ERROR_OK) goto err;
  if(bgAStarHeapCreate(&heap, header.node_count) != BG_ERROR_OK) goto err;
  if(bgPathCreate(&path, header.node_count) != BG_ERROR_OK) goto err;

  // 重みを0で初期化
  for(bgIdx_t i = 0; i < nc.dim; i++) {
    bgNodeContextSetWeight(&nc, i, 0);
  }
  for(bgIdx_t i = 0; i < wc.dim; i++) {
    bgWayContextSetWeight(&wc, i, 0);
  }

  return Result::Ok();

err:
  bgReaderDestroy(&reader);
  bgHeaderDestroy(&header);
  bgNodeContextDestroy(&nc);
  bgWayContextDestroy(&wc);
  bgNodeDestroy(&node);
  bgWayDestroy(&way);
  bgAStarVisitedDestroy(&visited);
  bgAStarHeapDestroy(&heap);
  bgPathDestroy(&path);

  return Result::Err("bg initialization failed.");
}

Result bg_astar(bgIdx_t start_node_idx, bgIdx_t goal_node_idx, bgCost_t heuristic_weight) {
  bgAStarVisitedClear(&visited);
  bgAStarHeapClear(&heap);
  bgPathClear(&path);

  bgError_t err = bgAStar(
    &reader, &header,
    &nc, &wc,
    &node, &way,
    &visited, &heap,
    start_node_idx, goal_node_idx,
    heuristic_weight,
    &path
  );

  switch(err) {
    case BG_ERROR_OK:
      return Result::Ok();
    case BG_ERROR_LOGIC:
      return Result::Err("The start node and the goal node are not connected.");
    case BG_ERROR_CAP:
      return Result::Err("bgAStar memory limit exceeded.");
  }
  return Result::Err("Unknown bgAStar error.");
}

};
