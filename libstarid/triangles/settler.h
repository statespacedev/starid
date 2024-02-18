#ifndef SETTLER_H
#define SETTLER_H

#include "include/Eigen/Core"
#include "triangles/starpairs.h"
#include "triangles/startriangle.h"

namespace starid {

    class SETTLER {
    public:
        explicit SETTLER(Starpairs &pairs);

        int run(Eigen::MatrixXd &pixels);

    private:
        bool get_angs_c();

        bool get_angs_d();

        Starpairs starpairs;
        Eigen::MatrixXd starvecs;
        double min_ang;
        std::vector<double> angs_c, angs_d;
        Eigen::Vector3d uveca, uvecb, uvecc, uvecd;
        int ndxb, ndxc, ndxd;
    };

}

#endif
