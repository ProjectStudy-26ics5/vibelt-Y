#include "../gui.hpp"
#include "../peripherals.hpp"

namespace vb {

GuiKind current_gui = GuiKind::Root;

static Result gui_setup() {
  switch(current_gui) {
    case GuiKind::Root:
      return gui_root_setup();

    case GuiKind::WeightConfig:
      return gui_weight_config_setup();
    
    case GuiKind::AStar:
      return gui_astar_setup();

    default:
      return Result::Err("Unknown gui kind.");
  }
}

Result gui_change(GuiKind gui) {
  current_gui = gui;

  Result result = gui_setup();
  if(result.ok()) {
    // GUIを遷移したときに、共通でちょっと待つようにします
    // これは、高速に連続でGUIが遷移され続けるのを防ぐためです。
    delay(500);
  }

  return result;
}

Result gui_runtime() {
  switch(current_gui) {
    case GuiKind::Root:
      return gui_root_runtime();

    case GuiKind::WeightConfig:
      return gui_weight_config_runtime();
    
    case GuiKind::AStar:
      return gui_astar_runtime();
    
    default:
      return Result::Err("Unknown gui kind.");
  }
}

};
