#include "../include/beltgraph.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUCKET_END_SIGN(visited) ((visited)->capacity)
#define BUCKET_UNALLOCATED_SIGN 0
#define UNALLOCATED_BUCKET(visited, i) ((visited)->chain[i] == BUCKET_UNALLOCATED_SIGN)

bgError_t bgAStarVisitedCreate(bgAStarVisited_t *visited, bgIdx_t table_size, bgIdx_t chain_size) {
  const bgIdx_t capacity = table_size + chain_size;

  visited->hash = malloc(capacity * sizeof(bgIdx_t));
  visited->chain = malloc(capacity * sizeof(bgIdx_t));
  visited->node_transition = malloc(capacity * sizeof(uint8_t));
  visited->capacity = capacity;
  visited->size = 0;
  visited->table_size = table_size;
  visited->chain_top = table_size;

  if(visited->hash == NULL) goto alloc_err;
  if(visited->chain == NULL) goto alloc_err;
  if(visited->node_transition == NULL) goto alloc_err;

  for(bgIdx_t i = 0; i < capacity; i++) {
    visited->chain[i] = BUCKET_UNALLOCATED_SIGN;
  }

  return BG_ERROR_OK;

alloc_err:
  bgAStarVisitedDestroy(visited);
  return BG_ERROR_ALLOC;
}

void bgAStarVisitedDestroy(bgAStarVisited_t *visited) {
  free(visited->hash);
  free(visited->chain);
  free(visited->node_transition);

  visited->hash = NULL;
  visited->chain = NULL;
  visited->node_transition = NULL;
  visited->capacity = 0;
  visited->size = 0;
  visited->table_size = 0;
  visited->chain_top = 0;
}

void bgAStarVisitedClear(bgAStarVisited_t *visited) {
  visited->size = 0;
  visited->chain_top = visited->table_size;

  for(bgIdx_t i = 0; i < visited->capacity; i++) {
    visited->chain[i] = BUCKET_UNALLOCATED_SIGN;
  }
}

int bgAStarVisitedFind(const bgAStarVisited_t *visited, bgIdx_t hash, uint8_t *node_transition) {
  for(bgIdx_t i = hash % visited->table_size; i != BUCKET_END_SIGN(visited); i = visited->chain[i]) {
    if(UNALLOCATED_BUCKET(visited, i)) break;

    if(visited->hash[i] == hash) {
      if(node_transition != NULL) {
        *node_transition = visited->node_transition[i];
      }
      return 1;
    }
  }

  return 0;
}

bgError_t bgAStarVisitedInsert(bgAStarVisited_t *visited, bgIdx_t hash, uint8_t node_transition) {
  // 要素がすでに存在しても追加します
  bgIdx_t i = hash % visited->table_size;

  if(UNALLOCATED_BUCKET(visited, i)) {
    visited->hash[i] = hash;
    visited->chain[i] = BUCKET_END_SIGN(visited);
    visited->node_transition[i] = node_transition;

    visited->size++;
    return BG_ERROR_OK;
  }

  if(visited->chain_top == visited->capacity) return BG_ERROR_CAP;
  bgIdx_t j = visited->chain_top++;

  visited->hash[j] = hash;
  visited->chain[j] = visited->chain[i];
  visited->node_transition[j] = node_transition;

  visited->chain[i] = j;

  visited->size++;
  return BG_ERROR_OK;
}



bgError_t bgAStarHeapCreate(bgAStarHeap_t *heap, bgIdx_t capacity) {
  heap->weight = malloc(capacity * sizeof(bgCost_t));
  heap->cost = malloc(capacity * sizeof(bgCost_t));
  heap->node_idx = malloc(capacity * sizeof(bgIdx_t));
  heap->node_transition = malloc(capacity * sizeof(uint8_t));
  heap->capacity = capacity;
  heap->size = 0;
  heap->peak_size = 0;

  if(heap->weight == NULL) goto alloc_err;
  if(heap->cost == NULL) goto alloc_err;
  if(heap->node_idx == NULL) goto alloc_err;

  return BG_ERROR_OK;

alloc_err:
  bgAStarHeapDestroy(heap);
  return BG_ERROR_ALLOC;
}

void bgAStarHeapDestroy(bgAStarHeap_t *heap) {
  free(heap->weight);
  free(heap->cost);
  free(heap->node_idx);
  free(heap->node_transition);

  heap->weight = NULL;
  heap->cost = NULL;
  heap->node_idx = NULL;
  heap->node_transition = NULL;
  heap->capacity = 0;
  heap->size = 0;
  heap->peak_size = 0;
}

void bgAStarHeapClear(bgAStarHeap_t *heap) {
  heap->size = 0;
  heap->peak_size = 0;
}

bgError_t bgAStarHeapPop(bgAStarHeap_t *heap) {
  if(heap->size == 0) {
    return BG_ERROR_LOGIC;
  }

  const bgIdx_t back = --heap->size;
  bgIdx_t p = 0;
  bgIdx_t c;

  while((c = (p << 1) + 1) < heap->size) {
    if(c + 1 < heap->size && heap->weight[c] > heap->weight[c + 1]) {
      c = c + 1;
    }
    if(heap->weight[back] <= heap->weight[c]) break;

    heap->weight[p] = heap->weight[c];
    heap->cost[p] = heap->cost[c];
    heap->node_idx[p] = heap->node_idx[c];
    heap->node_transition[p] = heap->node_transition[c];

    p = c;
  }

  heap->weight[p] = heap->weight[back];
  heap->cost[p] = heap->cost[back];
  heap->node_idx[p] = heap->node_idx[back];
  heap->node_transition[p] = heap->node_transition[back];

  return BG_ERROR_OK;
}

bgError_t bgAStarHeapPush(
  bgAStarHeap_t *heap,
  bgCost_t weight,
  bgCost_t cost, bgIdx_t node_idx, uint8_t node_transition
) {
  if(heap->size == heap->capacity) {
    return BG_ERROR_CAP;
  }

  const bgIdx_t back = heap->size++;
  bgIdx_t p;
  bgIdx_t c = back;

  while(c != 0) {
    p = (c - 1) >> 1;
    if(heap->weight[p] <= weight) break;

    heap->weight[c] = heap->weight[p];
    heap->cost[c] = heap->cost[p];
    heap->node_idx[c] = heap->node_idx[p];
    heap->node_transition[c] = heap->node_transition[p];
    
    c = p;
  }

  heap->weight[c] = weight;
  heap->cost[c] = cost;
  heap->node_idx[c] = node_idx;
  heap->node_transition[c] = node_transition;

  if(heap->peak_size < heap->size) {
    heap->peak_size = heap->size;
  }
  return BG_ERROR_OK;
}

bgError_t bgAStarHeapTop(
  const bgAStarHeap_t *heap,
  bgCost_t *weight,
  bgCost_t *cost, bgIdx_t *node_idx, uint8_t *node_transition
) {
  if(heap->size == 0) {
    return BG_ERROR_LOGIC;
  }

  if(weight != NULL) {
    *weight = heap->weight[0];
  }
  if(cost != NULL) {
    *cost = heap->cost[0];
  }
  if(node_idx != NULL) {
    *node_idx = heap->node_idx[0];
  }
  if(node_transition != NULL) {
    *node_transition = heap->node_transition[0];
  }

  return BG_ERROR_OK;
}



bgError_t bgPathCreate(bgPath_t *path, bgIdx_t capacity) {
  path->node_idx_order = malloc(capacity * sizeof(bgIdx_t));
  path->capacity = capacity;
  path->size = 0;

  if(path->node_idx_order == NULL) goto alloc_err;

  return BG_ERROR_OK;

alloc_err:
  bgPathDestroy(path);
  return BG_ERROR_ALLOC;
}

void bgPathDestroy(bgPath_t *path) {
  free(path->node_idx_order);

  path->node_idx_order = NULL;
  path->capacity = 0;
  path->size = 0;
}

void bgPathClear(bgPath_t *path) {
  path->size = 0;
}

bgError_t bgPathPush(bgPath_t *path, bgIdx_t node_idx) {
  if(path->size == path->capacity) {
    return BG_ERROR_CAP;
  }

  path->node_idx_order[path->size++] = node_idx;
  
  return BG_ERROR_OK;
}

void bgPathReverse(bgPath_t *path) {
  bgIdx_t i, j;
  bgIdx_t tmp;
  
  for(i = 0; i < (path->size >> 1); i++) {
    j = path->size - 1 - i;

    tmp = path->node_idx_order[i];
    path->node_idx_order[i] = path->node_idx_order[j];
    path->node_idx_order[j] = tmp;
  }
}



static bgCost_t heuristic(const bgNode_t *node, double goal_lon, double goal_lat, bgCost_t weight_sqr) {
  bgCost_t manhattan_dist = fabs(goal_lon - node->lon) + fabs(goal_lat - node->lat);
  return manhattan_dist * weight_sqr;
}

bgError_t bgAStar(
  bgReader_t *reader, const bgHeader_t *header,
  const bgNodeContext_t *nc, const bgWayContext_t *wc,
  bgNode_t *node, bgWay_t *way,
  bgAStarVisited_t *visited, bgAStarHeap_t *heap,
  bgIdx_t start_node_idx, bgIdx_t goal_node_idx,
  bgCost_t heuristic_weight,
  bgPath_t *path
) {
  bgError_t err;
  bgCost_t g, h;
  bgIdx_t node_idx;
  bgIdx_t way_begin, way_end;
  bgIdx_t way_idx;
  uint8_t node_transition;
  double goal_lon, goal_lat;
  bgCost_t weight_sqr;

  // ゴール情報を取り出す(heuristicで使うから)
  bgNodeFetch(node, reader, header, goal_node_idx); // nodeはゴールノード
  goal_lon = node->lon;
  goal_lat = node->lat;

  weight_sqr = 1;
  for(bgIdx_t i = 0; i < nc->dim; i++) {
    weight_sqr += nc->weights[i] * nc->weights[i];
  }
  for(bgIdx_t i = 0; i < wc->dim; i++) {
    weight_sqr += wc->weights[i] * wc->weights[i];
  }

  // スタートを最初の後継として追加
  err = bgAStarHeapPush(heap, 0, 0, start_node_idx, 0);
  if(err != BG_ERROR_OK) return err;

  /* A* 探索ループ */
  for(;;) {
    if(heap->size == 0) return BG_ERROR_LOGIC;
    // ヒープ重みが最小の後継を取り出す
    bgAStarHeapTop(heap, NULL, &g, &node_idx, &node_transition);
    bgAStarHeapPop(heap);

    // すでに到達したノードならスキップ、そうでなければ到達を記録
    if(bgAStarVisitedFind(visited, node_idx, NULL)) continue;
    err = bgAStarVisitedInsert(visited, node_idx, node_transition);
    if(err != BG_ERROR_OK) return err;

    // ゴール到達!
    if(node_idx == goal_node_idx) {
      break;
    }

    // 現在ノード情報を取り出す(持ってるウェイを調べるため)
    bgNodeFetch(node, reader, header, node_idx); // nodeは現在ノード
    way_begin = node->way_begin;
    way_end = node->way_end;

    // 現在ノードが持つすべてのウェイに対して
    for(way_idx = way_begin; way_idx < way_end; way_idx++) {
      bgWayFetch(way, reader, header, way_idx); // wayは現在ノードが持つウェイ
      if(bgAStarVisitedFind(visited, way->to, NULL)) continue;
      bgNodeFetch(node, reader, header, way->to); // nodeは後継ノード

      // コスト計算
      g = g + bgNodeCost(node, nc) + bgWayCost(way, wc);
      h = heuristic(node, goal_lon, goal_lat, weight_sqr) * heuristic_weight;

      // 後継を追加
      node_transition = way->reverse_idx - node->way_begin;
      err = bgAStarHeapPush(heap, g + h, g, node->idx, node_transition);
      if(err != BG_ERROR_OK) return err;
    }
  }

  /* path作成 */
  for(node_idx = goal_node_idx; node_idx != start_node_idx; node_idx = way->to) { // ゴールからスタートまで、node_transitionをたどる
    err = bgPathPush(path, node_idx); // pathに追加
    if(err != BG_ERROR_OK) return err;

    bgAStarVisitedFind(visited, node_idx, &node_transition);

    bgNodeFetch(node, reader, header, node_idx); // nodeは現在pathに追加したのの
    way_idx = node->way_begin + node_transition;
    bgWayFetch(way, reader, header, way_idx); // wayはnode_transitionが示すもの
  }
  err = bgPathPush(path, start_node_idx);
  if(err != BG_ERROR_OK) return err;
  
  // ゴールからスタートの順なので、反転する
  bgPathReverse(path);

  return BG_ERROR_OK;
}
