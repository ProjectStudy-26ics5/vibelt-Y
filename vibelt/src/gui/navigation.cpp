#include "../gui.hpp"
#include "../peripherals.hpp"
#include "../navigation.hpp"

namespace vb {

Result gui_navigation_setup() {
  // メモ: navigating = true;
  
  return Result::Ok();
}

Result gui_navigation_runtime() {
  // メモ: 終了ボタンが押されると navigating = false;
  
  return Result::Ok();
}

};
