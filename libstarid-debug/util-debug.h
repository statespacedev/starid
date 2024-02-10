#ifndef STARID_UTIL_DEBUG_H
#define STARID_UTIL_DEBUG_H

#include "starpairs.h"
#include "sky.h"

starid::Sky sky_util(std::string &pathsky, std::string &pathskymap);

starid::Starpairs starpairs_util(starid::Sky &sky, std::string &pathstarpairs);

#endif //STARID_UTIL_DEBUG_H
