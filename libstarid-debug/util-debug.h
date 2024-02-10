#ifndef STARID_UTIL_DEBUG_H
#define STARID_UTIL_DEBUG_H

#include "starpairs.h"
#include "sky.h"

starid::Sky read_sky(std::string &pathsky, std::string &pathcat);

starid::Starpairs read_starpairs(starid::Sky &sky, std::string &pathstarpairs);

#endif //STARID_UTIL_DEBUG_H
