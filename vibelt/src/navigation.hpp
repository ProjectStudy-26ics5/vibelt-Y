#ifndef VIBELT_NAVIGATION_HPP
#define VIBELT_NAVIGATION_HPP

#include "common.hpp"

namespace vb {

extern bool navigating;
extern double dest_lon;
extern double dest_lat;

Result navigation_runtime();

};

#endif
