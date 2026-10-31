#include "../include/beltgraph.h"

bgError_t bgHeaderCreate(bgHeader_t *header) {
  return BG_ERROR_OK;
}

void bgHeaderDestroy(bgHeader_t *header) {
  header->nc_pos = 0;
  header->nc_dim = 0;
  header->nc_ind_name_size = 0;
  header->nc_ind_desc_size = 0;

  header->wc_pos = 0;
  header->wc_dim = 0;
  header->wc_ind_name_size = 0;
  header->wc_ind_desc_size = 0;

  header->node_pos = 0;
  header->node_count = 0;
  header->node_stride = 0;

  header->way_pos = 0;
  header->way_count = 0;
  header->way_stride = 0;
}

void bgHeaderFetch(bgHeader_t *header, bgReader_t *reader) {
  bgReaderJump(reader, BG_HEADER_POS);

  bgReaderRead(&header->nc_pos, sizeof(bgOffset_t), 1, reader);
  bgReaderRead(&header->nc_dim, sizeof(bgIdx_t), 1, reader);
  bgReaderRead(&header->nc_ind_name_size, sizeof(bgSize_t), 1, reader);
  bgReaderRead(&header->nc_ind_desc_size, sizeof(bgSize_t), 1, reader);
  
  bgReaderRead(&header->wc_pos, sizeof(bgOffset_t), 1, reader);
  bgReaderRead(&header->wc_dim, sizeof(bgIdx_t), 1, reader);
  bgReaderRead(&header->wc_ind_name_size, sizeof(bgSize_t), 1, reader);
  bgReaderRead(&header->wc_ind_desc_size, sizeof(bgSize_t), 1, reader);

  bgReaderRead(&header->node_pos, sizeof(bgOffset_t), 1, reader);
  bgReaderRead(&header->node_count, sizeof(bgIdx_t), 1, reader);
  bgReaderRead(&header->node_stride, sizeof(bgSize_t), 1, reader);

  bgReaderRead(&header->way_pos, sizeof(bgOffset_t), 1, reader);
  bgReaderRead(&header->way_count, sizeof(bgIdx_t), 1, reader);
  bgReaderRead(&header->way_stride, sizeof(bgSize_t), 1, reader);
}

#ifdef GRAPHER
void bgHeaderWrite(const bgHeader_t *header, FILE *out) {
  fseek(
    out,
    BG_HEADER_POS,
    SEEK_SET
  );

  /**
   * [!WARNING]
   * エンディアンを揃えた方が良いです
   */

  fwrite(&header->nc_pos, sizeof(bgOffset_t), 1, out);
  fwrite(&header->nc_dim, sizeof(bgIdx_t), 1, out);
  fwrite(&header->nc_ind_name_size, sizeof(bgSize_t), 1, out);
  fwrite(&header->nc_ind_desc_size, sizeof(bgSize_t), 1, out);
  
  fwrite(&header->wc_pos, sizeof(bgOffset_t), 1, out);
  fwrite(&header->wc_dim, sizeof(bgIdx_t), 1, out);
  fwrite(&header->wc_ind_name_size, sizeof(bgSize_t), 1, out);
  fwrite(&header->wc_ind_desc_size, sizeof(bgSize_t), 1, out);

  fwrite(&header->node_pos, sizeof(bgOffset_t), 1, out);
  fwrite(&header->node_count, sizeof(bgIdx_t), 1, out);
  fwrite(&header->node_stride, sizeof(bgSize_t), 1, out);

  fwrite(&header->way_pos, sizeof(bgOffset_t), 1, out);
  fwrite(&header->way_count, sizeof(bgIdx_t), 1, out);
  fwrite(&header->way_stride, sizeof(bgSize_t), 1, out);
}

bgSize_t bgHeaderByteCount(void) {
  bgSize_t size = 0;

  size += sizeof(bgOffset_t);
  size += sizeof(bgIdx_t);
  size += sizeof(bgSize_t);
  size += sizeof(bgSize_t);
  
  size += sizeof(bgOffset_t);
  size += sizeof(bgIdx_t);
  size += sizeof(bgSize_t);
  size += sizeof(bgSize_t);

  size += sizeof(bgOffset_t);
  size += sizeof(bgIdx_t);
  size += sizeof(bgSize_t);
  
  size += sizeof(bgOffset_t);
  size += sizeof(bgIdx_t);
  size += sizeof(bgSize_t);

  return size;
}
#endif
