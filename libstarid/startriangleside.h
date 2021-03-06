#ifndef STARID_STARTRIANGLESIDE_H
#define STARID_STARTRIANGLESIDE_H
#include <unordered_map>
#include <map>
#include <vector>
#include "starpairs.h"

namespace starid {

class Startriangleside {
public:
    Startriangleside(double ang, starid::Starpairs &pairs);
    Startriangleside();
    void drops(bool dolog = true);
    void update(Startriangleside &side);
    int countpairs();
    std::unordered_map<int, std::unordered_map<int, int>> stars;
    std::vector<int> log_star_count;
    std::vector<int> log_pair_count;
    double angtol;
};

}


#endif //STARID_STARTRIANGLESIDE_H
