#ifndef STARID_STAR_PAIRS_H
#define STARID_STAR_PAIRS_H

#include "include/Eigen/Core"
#include "include/cereal/access.hpp"
#include "include/cereal/types/vector.hpp"
#include "include/cereal/types/string.hpp"
#include "include/cereal/types/tuple.hpp"
#include "include/cereal/types/unordered_map.hpp"
#include <unordered_map>
#include <map>
#include "sky/sky.h"

namespace starid {

class Starpairs {

public:
    Starpairs();
    void generate(starid::Sky &sky);
    std::unordered_map<int, std::unordered_map<int, int>> pairs_for_angle(double angle, double tol_radius);

private:
    starid::FloatsIndexer angndxs;
    std::vector <std::tuple<double, int, int>> starpairs;
    std::unordered_map<std::string, int> starpairsndxs;

    std::string pair_labeler(int catndx1, int catndx2);

    friend class cereal::access;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(starpairs, starpairsndxs, angndxs);
    }
};

}

#endif //STARID_STAR_PAIRS_H