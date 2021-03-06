#ifndef STARID_STARPAIRS_H
#define STARID_STARPAIRS_H

#include <Eigen/Core>
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/unordered_map.hpp>
#include <unordered_map>
#include <map>
#include "sky.h"

namespace starid {

class Starpairs {

public:
    Starpairs();
    void generate(starid::Sky &sky);
    std::unordered_map<int, std::unordered_map<int, int>> pairs_for_angle(double angle, double tol_radius);

private:
    starid::range_of_floats_indexer angndxs;
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

#endif //STARID_STARPAIRS_H
