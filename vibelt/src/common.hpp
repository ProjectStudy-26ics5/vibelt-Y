#ifndef VIBELT_COMMON_HPP
#define VIBELT_COMMON_HPP

#include <stddef.h>
#include <stdint.h>
#include "beltgraph/include/beltgraph/types.h"

namespace vb {

/* エラー制御 */
enum class ResultKind {
  Ok,
  Err,
};

class Result {
public:
  Result(const Result &other) = default;
  Result(Result &&other) = default;
  Result &operator=(const Result &other) = default;
  Result &operator=(Result &&other) = default;
  
  static Result Ok(const char *msg__ = nullptr);
  static Result Err(const char *msg__);

  bool ok() const;
  const char *what() const;

private:
  Result(ResultKind kind__, const char *msg__);

  ResultKind kind_;
  const char *msg_;
};

void panic(const char *msg);
void check(Result result);



/* bg */
extern bgReader_t reader;
extern bgHeader_t header;
extern bgNodeContext_t nc;
extern bgWayContext_t wc;
extern bgNode_t node;
extern bgWay_t way;
extern bgAStarVisited_t visited;
extern bgAStarHeap_t heap;
extern bgPath_t path;

Result bg_setup();
Result bg_astar(bgIdx_t start_node_idx, bgIdx_t goal_node_idx, bgCost_t heuristic_weight);

};

#endif
