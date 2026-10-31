#ifndef BELTGRAPH_PATHFINDER_H
#define BELTGRAPH_PATHFINDER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bgAStarVisited {
  // SoAの構造です。
  
  // hash値はノードのidxです
  bgIdx_t *hash;

  // chainは
  // bgAStarVisited::sizeであるときは、chain終了
  // 0であるときは、このバケットが未割当
  bgIdx_t *chain;

  // Nodeが持っているWayのうち、前回のノードにつながるもの
  // 出次数が255を超えることはめったにないため
  // 8bit(1byte)のデータに圧縮できます
  uint8_t *node_transition;

  bgIdx_t capacity;
  bgIdx_t size;
  bgIdx_t table_size;
  bgIdx_t chain_top;
};

bgError_t bgAStarVisitedCreate(bgAStarVisited_t *visited, bgIdx_t table_size, bgIdx_t chain_size);
void bgAStarVisitedDestroy(bgAStarVisited_t *visited);
void bgAStarVisitedClear(bgAStarVisited_t *visited);
int bgAStarVisitedFind(const bgAStarVisited_t *visited, bgIdx_t hash, uint8_t *node_transition);
bgError_t bgAStarVisitedInsert(bgAStarVisited_t *visited, bgIdx_t hash, uint8_t node_transition);



struct bgAStarHeap {
  // SoAの構造です。

  // 指標値は厳密な数値ではないので、
  // weightとcostは半精度浮動小数点を使えるかもしれません
  bgCost_t *weight;
  bgCost_t *cost;
  bgIdx_t *node_idx;
  uint8_t *node_transition;

  bgIdx_t capacity;
  bgIdx_t size;
  bgIdx_t peak_size;
};

bgError_t bgAStarHeapCreate(bgAStarHeap_t *heap, bgIdx_t capacity);
void bgAStarHeapDestroy(bgAStarHeap_t *heap);
void bgAStarHeapClear(bgAStarHeap_t *heap);
bgError_t bgAStarHeapPop(bgAStarHeap_t *heap);
bgError_t bgAStarHeapPush(
  bgAStarHeap_t *heap,
  bgCost_t weight,
  bgCost_t cost, bgIdx_t node_idx, uint8_t node_transition
);
bgError_t bgAStarHeapTop(
  const bgAStarHeap_t *heap,
  bgCost_t *weight,
  bgCost_t *cost, bgIdx_t *node_idx, uint8_t *node_transition
);



struct bgPath {
  bgIdx_t *node_idx_order;
  bgIdx_t capacity;
  bgIdx_t size;
};

bgError_t bgPathCreate(bgPath_t *path, bgIdx_t capacity);
void bgPathDestroy(bgPath_t *path);
void bgPathClear(bgPath_t *path);
bgError_t bgPathPush(bgPath_t *path, bgIdx_t node_idx);
void bgPathReverse(bgPath_t *path);



bgError_t bgAStar(
  bgReader_t *reader, const bgHeader_t *header,
  const bgNodeContext_t *nc, const bgWayContext_t *wc,
  bgNode_t *node, bgWay_t *way,
  bgAStarVisited_t *visited, bgAStarHeap_t *heap,
  bgIdx_t start_node_idx, bgIdx_t goal_node_idx,
  bgCost_t heuristic_weight,
  bgPath_t *path
);

#ifdef __cplusplus
}
#endif

#endif
