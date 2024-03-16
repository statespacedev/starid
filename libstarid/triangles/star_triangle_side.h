#ifndef STARID_STAR_TRIANGLE_SIDE_H
#define STARID_STAR_TRIANGLE_SIDE_H

#include <unordered_map>
#include <map>
#include <vector>
#include "star_pairs.h"
#include "../sky/geometry.h"

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

    class Startriangleside2 {
    public:
        Startriangleside2(Eigen::Vector3d sv1_, Eigen::Vector3d sv2_, starid::Starpairs &pairs);

        Startriangleside2& operator=(Startriangleside2 other);

        Startriangleside2();

//        void drops(bool dolog = true);

//        void update(Startriangleside &side);

        int countpairs();

        Eigen::Vector3d sv1, sv2;
        double ang;
        std::unordered_map<int, std::unordered_map<int, int>> stars;
        std::vector<int> log_star_count;
        std::vector<int> log_pair_count;
    private:
        double angtol;
    };

}


#endif //STARID_STAR_TRIANGLE_SIDE_H
