#ifndef STARID_STARTRIANGLESIDE_H
#define STARID_STARTRIANGLESIDE_H
#include <unordered_map>
#include <map>
#include <vector>
#include "starpairs.h"

namespace starid {

class Startriangleside {
public:
    std::unordered_map<int, std::unordered_map<int, int>> stars;

    void trim_pairs();

    void update(Startriangleside &side);

    std::vector<int> log_star_count;
    std::vector<int> log_pair_count;

    Startriangleside(double ang, double tolerance, starid::Starpairs &pairs);

    Startriangleside();

    int pair_count();

private:
};

}


#endif //STARID_STARTRIANGLESIDE_H
