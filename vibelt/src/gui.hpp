#ifndef VIBELT_GUI_HPP
#define VIBELT_GUI_HPP

#include "common.hpp"

namespace vb {

enum class GuiKind {
  Root,
  WeightConfig,
  AStar,
};
extern GuiKind current_gui;

Result gui_change(GuiKind gui);
Result gui_runtime();



Result gui_root_setup();
Result gui_root_runtime();

Result gui_weight_config_setup();
Result gui_weight_config_runtime();

Result gui_astar_setup();
Result gui_astar_runtime();

};

#endif
