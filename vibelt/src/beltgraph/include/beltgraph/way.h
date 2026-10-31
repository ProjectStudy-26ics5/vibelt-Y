#ifndef BELTGRAPH_WAY_H
#define BELTGRAPH_WAY_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bgWayContext {
  bgIdx_t dim;
  bgWeight_t *weights;

  bgSize_t indicator_name_size;
  char *indicator_names;
  bgOffset_t *indicator_name_offs;

  bgSize_t indicator_desc_size;
  char *indicator_descs;
  bgOffset_t *indicator_desc_offs;
};

struct bgWay {
  bgIdx_t idx;
  bgIdx_t reverse_idx;
  bgIdx_t to;
  double length;
  bgIndicator_t *indicators;
};

bgError_t bgWayContextCreate(bgWayContext_t *wc, const bgHeader_t *header);
void bgWayContextDestroy(bgWayContext_t *wc);
void bgWayContextFetch(bgWayContext_t *wc, bgReader_t *reader, const bgHeader_t *header);
const char *bgWayContextIndicatorName(const bgWayContext_t *wc, bgIdx_t i);
const char *bgWayContextIndicatorDesc(const bgWayContext_t *wc, bgIdx_t i);
void bgWayContextSetWeight(bgWayContext_t *wc, bgIdx_t i, bgWeight_t weight);
void bgWayContextSetWeightPreset(bgWayContext_t *wc, const bgWeight_t *weights);

bgError_t bgWayCreate(bgWay_t *way, const bgHeader_t *header);
void bgWayDestroy(bgWay_t *way);
void bgWayFetch(bgWay_t *way, bgReader_t *reader, const bgHeader_t *header, bgIdx_t idx);
bgCost_t bgWayCost(const bgWay_t *way, const bgWayContext_t *wc);

#ifdef GRAPHER
void bgWayContextWrite(const bgWayContext_t *wc, FILE *out, const bgHeader_t *header);
bgSize_t bgWayContextByteCount(bgIdx_t wc_dim, bgSize_t wc_ind_name_size, bgSize_t wc_ind_desc_size);

void bgWayWrite(const bgWay_t *way, FILE *out, const bgHeader_t *header);
bgSize_t bgWayByteCount(bgIdx_t wc_dim);
#endif

#ifdef __cplusplus
}
#endif

#endif
