#ifndef BELTGRAPH_HEADER_H
#define BELTGRAPH_HEADER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BG_HEADER_POS 0

struct bgHeader {
  bgOffset_t nc_pos;
  bgIdx_t nc_dim;
  bgSize_t nc_ind_name_size;
  bgSize_t nc_ind_desc_size;
  
  bgOffset_t wc_pos;
  bgIdx_t wc_dim;
  bgSize_t wc_ind_name_size;
  bgSize_t wc_ind_desc_size;

  bgOffset_t node_pos;
  bgIdx_t node_count;
  bgSize_t node_stride;

  bgOffset_t way_pos;
  bgIdx_t way_count;
  bgSize_t way_stride;
};

bgError_t bgHeaderCreate(bgHeader_t *header);
void bgHeaderDestroy(bgHeader_t *header);
void bgHeaderFetch(bgHeader_t *header, bgReader_t *reader);

#ifdef GRAPHER
void bgHeaderWrite(const bgHeader_t *header, FILE *out);
bgSize_t bgHeaderByteCount(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
