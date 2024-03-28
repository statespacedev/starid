#ifndef SETTLER_H
#define SETTLER_H

#include "Eigen/Core"
#include "star_pairs.h"
#include "settler_triangle.h"
using namespace starid;
using namespace Eigen;

namespace starid {
    class SETTLER {
    public:
        explicit SETTLER(Starpairs &pairs);
        int run(MatrixXd &pixels);
    private:
        bool get_angs_c();
        bool get_angs_d();
        Starpairs starpairs;
        MatrixXd starvecs;
        double min_ang;
        std::vector<double> angs_c, angs_d;
        Vector3d uveca, uvecb, uvecc, uvecd;
        int ndxb, ndxc, ndxd;
    };
}

#endif
