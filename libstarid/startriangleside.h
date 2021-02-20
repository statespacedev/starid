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

    void append_iterations(Startriangleside &side);

    std::vector<int> log_star_count;
    std::vector<int> log_pair_count;
    std::vector<bool> log_teststar;
    int teststar;
    bool has_teststar;

    Startriangleside(double ang, double tolerance, starid::Starpairs &pairs, int teststar);

    std::map<int, int> summary();

    bool check_teststar(int teststar);

    int pair_count();

private:
};

}


#endif //STARID_STARTRIANGLESIDE_H
