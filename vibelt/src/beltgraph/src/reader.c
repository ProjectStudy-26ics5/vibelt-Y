#include "../include/beltgraph.h"

bgError_t bgReaderCreate(bgReader_t *reader, bgReaderCallback cb) {
  reader->pos = 0;
  reader->cb = cb;

  return BG_ERROR_OK;
}

void bgReaderDestroy(bgReader_t *reader) {
  reader->pos = 0;
  reader->cb = NULL;
}

void bgReaderJump(bgReader_t *reader, bgOffset_t pos) {
  reader->pos = pos;
}

void bgReaderRead(void *buffer, bgSize_t size, bgIdx_t count, bgReader_t *reader) {
  bgSize_t byte_count = size * count;
  
  reader->cb(reader->pos, buffer, byte_count);
  reader->pos += byte_count;
}
