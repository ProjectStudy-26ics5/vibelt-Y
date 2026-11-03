#ifndef VIBELT_TYPES_HPP
#define VIBELT_TYPES_HPP

#include <stddef.h>
#include <stdint.h>
#include "src/beltgraph/include/beltgraph/types.h"

/**
 * NULLだったらOK
 * 文字列が入ったらERROR
 */
#define VIBELT_OK NULL
typedef const char *vibelt_result_t;

void vibelt_panic(vibelt_result_t err);
void vibelt_check(vibelt_result_t err);

#endif
