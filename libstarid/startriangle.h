#ifndef STARID_STARTRIANGLE_H
#define STARID_STARTRIANGLE_H
#include "startriangleside.h"

namespace starid {

class Startriangle {
    public:
        Startriangle(int basestarndx, Eigen::MatrixXd &starvecs);
        Startriangle(double ang1, double ang2, double ang3, double tolerance,
                     starid::Starpairs &pairs, int teststar, Eigen::Vector3d vecstar3);
        void close_loops_abca();
        void close_loops_abda(std::vector<Startriangle> &triangles, starid::Starpairs &pairs);
        starid::Startriangleside side1;
        starid::Startriangleside side2;
        starid::Startriangleside side3;
        int loops_cnt{};
        int teststar{};
        double tolerance{};
        Eigen::Vector3d vecstar3;
    private:
    };

}

#endif //STARID_STARTRIANGLE_H
