#include "../include/beltgraph.h"
#include <stdlib.h>
#include <string.h>

bgError_t bgWayContextCreate(bgWayContext_t *wc, const bgHeader_t *header) {
  const bgIdx_t dim = header->wc_dim;

  wc->dim = dim;
  wc->weights = malloc(dim * sizeof(bgWeight_t));

  wc->indicator_name_size = header->wc_ind_name_size;
  wc->indicator_names = malloc(header->wc_ind_name_size);
  wc->indicator_name_offs = malloc(dim * sizeof(bgOffset_t));
  
  wc->indicator_desc_size = header->wc_ind_desc_size;
  wc->indicator_descs = malloc(header->wc_ind_desc_size);
  wc->indicator_desc_offs = malloc(dim * sizeof(bgOffset_t));

  if(wc->indicator_names == NULL) goto alloc_err;
  if(wc->indicator_name_offs == NULL) goto alloc_err;
  if(wc->indicator_descs == NULL) goto alloc_err;
  if(wc->indicator_desc_offs == NULL) goto alloc_err;

  return BG_ERROR_OK;

alloc_err:
  bgWayContextDestroy(wc);
  return BG_ERROR_ALLOC;
}

void bgWayContextDestroy(bgWayContext_t *wc) {
  free(wc->weights);
  free(wc->indicator_names);
  free(wc->indicator_name_offs);
  free(wc->indicator_descs);
  free(wc->indicator_desc_offs);

  wc->dim = 0;
  wc->weights = NULL;

  wc->indicator_name_size = 0;
  wc->indicator_names = NULL;
  wc->indicator_name_offs = NULL;
  
  wc->indicator_desc_size = 0;
  wc->indicator_descs = NULL;
  wc->indicator_desc_offs = NULL;
}

void bgWayContextFetch(bgWayContext_t *wc, bgReader_t *reader, const bgHeader_t *header) {
  const bgIdx_t dim = header->wc_dim;

  bgReaderJump(reader, header->wc_pos);

  bgReaderRead(wc->indicator_names, wc->indicator_name_size, 1, reader);
  bgReaderRead(wc->indicator_name_offs, sizeof(bgOffset_t), dim, reader);
  
  bgReaderRead(wc->indicator_descs, wc->indicator_desc_size, 1, reader);
  bgReaderRead(wc->indicator_desc_offs, sizeof(bgOffset_t), dim, reader);
}

const char *bgWayContextIndicatorName(const bgWayContext_t *wc, bgIdx_t i) {
  return wc->indicator_names + wc->indicator_name_offs[i];
}

const char *bgWayContextIndicatorDesc(const bgWayContext_t *wc, bgIdx_t i) {
  return wc->indicator_descs + wc->indicator_desc_offs[i];
}

void bgWayContextSetWeight(bgWayContext_t *wc, bgIdx_t i, bgWeight_t weight) {
  wc->weights[i] = weight;
}

void bgWayContextSetWeightPreset(bgWayContext_t *wc, const bgWeight_t *weights) {
  memcpy(wc->weights, weights, wc->dim * sizeof(bgWeight_t));
}



bgError_t bgWayCreate(bgWay_t *way, const bgHeader_t *header) {
  const bgIdx_t dim = header->wc_dim;

  way->indicators = malloc(dim * sizeof(bgIndicator_t));

  if(way->indicators == NULL) goto alloc_err;

  return BG_ERROR_OK;

alloc_err:
  bgWayDestroy(way);
  return BG_ERROR_ALLOC;
}

void bgWayDestroy(bgWay_t *way) {
  free(way->indicators);

  way->idx = 0;
  way->reverse_idx = 0;
  way->to = 0;
  way->length = 0;
  way->indicators = NULL;
}

void bgWayFetch(bgWay_t *way, bgReader_t *reader, const bgHeader_t *header, bgIdx_t idx) {
  const bgIdx_t dim = header->wc_dim;
  way->idx = idx;

  bgReaderJump(
    reader,
    header->way_pos + header->way_stride * way->idx
  );

  bgReaderRead(&way->reverse_idx, sizeof(bgIdx_t), 1, reader);
  bgReaderRead(&way->to, sizeof(bgIdx_t), 1, reader);
  bgReaderRead(&way->length, sizeof(double), 1, reader);
  bgReaderRead(way->indicators, sizeof(bgIndicator_t), dim, reader);
}

bgCost_t bgWayCost(const bgWay_t *way, const bgWayContext_t *wc) {
  bgCost_t c = 1;

  for(bgIdx_t i = 0; i < wc->dim; i++) {
    if(wc->weights[i] > 0) {
      c += (1 - way->indicators[i]) * wc->weights[i];
    }else {
      c += (way->indicators[i]) * -wc->weights[i];
    }
  }

  c *= way->length;
  return c;
}



#ifdef GRAPHER
void bgWayContextWrite(const bgWayContext_t *wc, FILE *out, const bgHeader_t *header) {
  const bgIdx_t dim = header->wc_dim;

  fseek(
    out,
    header->wc_pos,
    SEEK_SET
  );

  /**
   * [!WARNING]
   * エンディアンを揃えた方が良いです
   */

  fwrite(wc->indicator_names, wc->indicator_name_size, 1, out);
  fwrite(wc->indicator_name_offs, sizeof(bgOffset_t), dim, out);
  
  fwrite(wc->indicator_descs, wc->indicator_desc_size, 1, out);
  fwrite(wc->indicator_desc_offs, sizeof(bgOffset_t), dim, out);
}

bgSize_t bgWayContextByteCount(bgIdx_t wc_dim, bgSize_t wc_ind_name_size, bgSize_t wc_ind_desc_size) {
  bgSize_t size = 0;

  size += wc_ind_name_size;
  size += sizeof(bgOffset_t) * wc_dim;

  size += wc_ind_desc_size;
  size += sizeof(bgOffset_t) * wc_dim;

  return size;
}



void bgWayWrite(const bgWay_t *way, FILE *out, const bgHeader_t *header) {
  const bgIdx_t dim = header->wc_dim;

  fseek(
    out,
    header->way_pos + header->way_stride * way->idx,
    SEEK_SET
  );

  fwrite(&way->reverse_idx, sizeof(bgIdx_t), 1, out);
  fwrite(&way->to, sizeof(bgIdx_t), 1, out);
  fwrite(&way->length, sizeof(double), 1, out);
  fwrite(way->indicators, sizeof(bgIndicator_t), dim, out);
}

bgSize_t bgWayByteCount(bgIdx_t wc_dim) {
  bgSize_t size = 0;

  size += sizeof(bgIdx_t);
  size += sizeof(bgIdx_t);
  size += sizeof(double);
  size += sizeof(bgIndicator_t) * wc_dim;
  
  return size;
}
#endif
