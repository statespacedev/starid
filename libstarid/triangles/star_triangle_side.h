#ifndef STARID_STAR_TRIANGLE_SIDE_H
#define STARID_STAR_TRIANGLE_SIDE_H

#include <unordered_map>
#include <map>
#include <vector>
#include <set>
#include "star_pairs.h"
#include "../sky/geometry.h"
using namespace Eigen;
using namespace starid;
using starsdict = std::unordered_map<int, std::unordered_map<int, int>>;
using starsdict2 = std::unordered_map<int, std::set<int>>;
using intvec = std::vector<int>;

namespace starid {
    class Startriangleside {
    public:
        Startriangleside(double, Starpairs&);
        Startriangleside();
        void drops(bool dolog = true);
        void update(Startriangleside &side);
        int countpairs();
        starsdict stars;
        intvec log_star_count;
        intvec log_pair_count;
        double angtol;
    };

    class Startriangleside2 {
    public:
        Startriangleside2(Vector3d, Vector3d, Starpairs&, float angtol = .003);
        Startriangleside2();
        void update_side(std::set<int>&);
        void update_abside(Startriangleside2&);
        void update_acands(std::set<int>&);
        int countpairs();
        Vector3d sv1, sv2;
        double ang;
        starsdict2 stars;
        intvec log_star_count;
        intvec log_pair_count;
    };
}


#endif //STARID_STAR_TRIANGLE_SIDE_H
