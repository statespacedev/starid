#ifndef STARID_STAR_PAIRS_H
#define STARID_STAR_PAIRS_H

#include "Eigen/Core"
#include "cereal/access.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/tuple.hpp"
#include "cereal/types/unordered_map.hpp"
#include <unordered_map>
#include <map>
#include <set>
#include "../sky/sky.h"
using namespace Eigen;
using namespace starid;
using starsdict2 = std::unordered_map<int, std::set<int>>;

namespace starid {
    class Starpairs {
    public:
        Starpairs();
        void generate(Sky &sky);
        starsdict2 pairs_for_angle(double angle, double tol_radius);
    private:
        FloatsIndexer angndxs;
        std::vector<std::tuple<double, int, int>> starpairs;
        std::unordered_map<std::string, int> starpairsndxs;
        std::string pair_labeler(int catndx1, int catndx2);
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive &ar) {ar(starpairs, starpairsndxs, angndxs);}
    };
}

#endif //STARID_STAR_PAIRS_H
