#ifndef STARID_SETTLER_TRIANGLE_H
#define STARID_SETTLER_TRIANGLE_H

#include "star_triangle_side.h"

namespace starid {

    class StartriangleSETTLER {
    public:
        StartriangleSETTLER(double ang1, double ang2, double ang3, starid::Starpairs &pairs, Eigen::Vector3d vecstar3);

        void constrain_abca();

        void constrain_abda(std::vector<StartriangleSETTLER> &triangles, starid::Starpairs &pairs);

        starid::Startriangleside side1, side2, side3;
        int loops_cnt{};
        Eigen::Vector3d vecstar3;
    };

}

#endif //STARID_SETTLER_TRIANGLE_H
