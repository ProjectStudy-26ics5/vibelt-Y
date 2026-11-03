#ifndef VIBELT_DPY_HPP
#define VIBELT_DPY_HPP

#include "types.hpp"

#define DPY_H_RES 320
#define DPY_V_RES 240

typedef enum gui_kind {
  GUI_NEUTRAL,
  GUI_WEIGHT
} gui_kind_t;

extern gui_kind_t gui_id;

/* 初期化 */
vibelt_result_t dpy_setup(void);

vibelt_result_t gui_neutral_setup(void);
vibelt_result_t gui_neutral_runtime(void);

vibelt_result_t gui_weight_setup(void);
vibelt_result_t gui_weight_runtime(void);

#endif
