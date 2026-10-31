#ifndef BELTGRAPH_READER_H
#define BELTGRAPH_READER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*bgReaderCallback)(bgOffset_t pos, void *buffer, bgSize_t size);

struct bgReader {
  bgOffset_t pos;
  bgReaderCallback cb;
};

bgError_t bgReaderCreate(bgReader_t *reader, bgReaderCallback cb);
void bgReaderDestroy(bgReader_t *reader);
void bgReaderJump(bgReader_t *reader, bgOffset_t pos);
void bgReaderRead(void *buffer, bgSize_t size, bgIdx_t count, bgReader_t *reader);

#ifdef __cplusplus
}
#endif

#endif
