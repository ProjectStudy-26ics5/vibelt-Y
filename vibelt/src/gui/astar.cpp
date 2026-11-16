#include "../gui.hpp"
#include "../peripherals.hpp"
#include "../navigation.hpp"
#include "../beltgraph/include/beltgraph.h"
#include <time.h>
#include <math.h>
#include <stdarg.h>

namespace vb {

static clock_t start;
static bool succeed;

static void print_ok(const char *fmt, ...) {
  dpy.print("[ ");
  dpy.setTextColor(ILI9341_GREEN);
  dpy.print("Ok");
  dpy.setTextColor(ILI9341_WHITE);
  dpy.print(" ] ");
  
  va_list args;
  va_start(args, fmt);
  dpy.printf(fmt, args);
  va_end(args);

  dpy.printf(" (%.4lf s)\n", (double)(clock() - start) / CLOCKS_PER_SEC);
}

static void print_err(const char *fmt, ...) {
  dpy.print("[ ");
  dpy.setTextColor(ILI9341_RED);
  dpy.print("Err");
  dpy.setTextColor(ILI9341_WHITE);
  dpy.print(" ] ");

  va_list args;
  va_start(args, fmt);
  dpy.printf(fmt, args);
  va_end(args);
  
  dpy.printf(" (%.4lf s)\n", (double)(clock() - start) / CLOCKS_PER_SEC);
}



Result gui_astar_setup() {
  dpy.fillScreen(ILI9341_BLACK);
  
  dpy.setTextColor(ILI9341_WHITE);
  dpy.setTextWrap(true);
  dpy.setTextSize(1);
  dpy.setCursor(0, 0);

  start = clock();
  bgIdx_t start_node_idx, goal_node_idx;


  dpy.print("Selecting start node...\n");
  if(!isnan(gps_lon) && !isnan(gps_lat)) {
    bgNodeFetchNearest(&node, &reader, &header, gps_lon, gps_lat);
    start_node_idx = node.idx;
    print_ok("start node idx is %u.", start_node_idx);

  }else {
    print_err("Unable to obtain coordinates from the GPS.");
    goto err;
  }


  dpy.print("Selecting goal node...\n");
  if(!isnan(dest_lon) && !isnan(dest_lat)) {
    bgNodeFetchNearest(&node, &reader, &header, dest_lon, dest_lat);
    goal_node_idx = node.idx;
    print_ok("goal node idx is %u.", goal_node_idx);

  }else {
    print_err("No destination has been specified.");
    goto err;
  }


  {
    dpy.print("Running astar...\n");
    Result astar_result = bg_astar(start_node_idx, goal_node_idx, 1.5);
    if(astar_result.ok()) {
      print_ok("path found.");

    }else {
      print_err("%s.", astar_result.what());
      goto err;
    }
  }

  dpy.printf("| visited | allocated: %u, used: %u\n", visited.capacity, visited.size);
  dpy.printf("| heap    | allocated: %u, used: %u\n", heap.capacity, heap.peak_size);
  dpy.printf("| path    | allocated: %u, used: %u\n", path.capacity, path.size);

  succeed = true;
  dpy.print("\nTouch display to navigation.\n");
  goto end;

err:
  succeed = false;
  dpy.print("\nTouch display to return.\n");
  goto end;

end:
  return Result::Ok();
}

Result gui_astar_runtime() {
  if(dpy_touched(
    0, 0,
    DPY_H_RES, DPY_V_RES
  )) {
    if(succeed) {
      return gui_change(GuiKind::Root); // あとで、ナビゲーション画面を作ります
    }else {
      return gui_change(GuiKind::Root);
    }
  }

  return Result::Ok();
}

};
