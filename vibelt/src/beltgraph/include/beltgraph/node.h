#ifndef BELTGRAPH_NODE_H
#define BELTGRAPH_NODE_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bgNodeContext {
  bgIdx_t dim;
  bgWeight_t *weights;

  bgSize_t indicator_name_size;
  char *indicator_names;
  bgOffset_t *indicator_name_offs;

  bgSize_t indicator_desc_size;
  char *indicator_descs;
  bgOffset_t *indicator_desc_offs;
};

struct bgNode {
  bgIdx_t idx;
  bgIdx_t way_begin;
  bgIdx_t way_end;
  double lon;
  double lat;
  bgIndicator_t *indicators;
};

bgError_t bgNodeContextCreate(bgNodeContext_t *nc, const bgHeader_t *header);
void bgNodeContextDestroy(bgNodeContext_t *nc);
void bgNodeContextFetch(bgNodeContext_t *nc, bgReader_t *reader, const bgHeader_t *header);
const char *bgNodeContextIndicatorName(const bgNodeContext_t *nc, bgIdx_t i);
const char *bgNodeContextIndicatorDesc(const bgNodeContext_t *nc, bgIdx_t i);
void bgNodeContextSetWeight(bgNodeContext_t *nc, bgIdx_t i, bgWeight_t weight);
void bgNodeContextSetWeightPreset(bgNodeContext_t *nc, const bgWeight_t *weights);

bgError_t bgNodeCreate(bgNode_t *node, const bgHeader_t *header);
void bgNodeDestroy(bgNode_t *node);
void bgNodeFetch(bgNode_t *node, bgReader_t *reader, const bgHeader_t *header, bgIdx_t idx);
void bgNodeFetchNearest(bgNode_t *node, bgReader_t *reader, const bgHeader_t *header, double lon, double lat);
bgCost_t bgNodeCost(const bgNode_t *node, const bgNodeContext_t *nc);

#ifdef GRAPHER
void bgNodeContextWrite(const bgNodeContext_t *nc, FILE *out, const bgHeader_t *header);
bgSize_t bgNodeContextByteCount(bgIdx_t nc_dim, bgSize_t nc_ind_name_size, bgSize_t nc_ind_desc_size);

void bgNodeWrite(const bgNode_t *node, FILE *out, const bgHeader_t *header);
bgSize_t bgNodeByteCount(bgIdx_t nc_dim);
#endif

#ifdef __cplusplus
}
#endif

#endif
