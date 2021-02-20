#ifndef STARTRIANGLES_H
#define STARTRIANGLES_H
#include <Eigen/Core>
#include "starpairs.h"

namespace starid {

class StartriangleIdentifier {
    public:
        explicit StartriangleIdentifier(Starpairs &pairs);
        int identify(Eigen::MatrixXd &pixels, int teststar = -1);
    private:
        bool get_angs_c();
        bool get_angs_d();
        Starpairs starpairs;
        Eigen::MatrixXd starvecs;
        double tolerance;
        double min_ang;
        std::vector<double> angs_c;
        std::vector<double> angs_d;
        Eigen::Vector3d uveca;
        Eigen::Vector3d uvecb;
        Eigen::Vector3d uvecc;
        Eigen::Vector3d uvecd;
        int ndxb, ndxc, ndxd;
    };

}

#endif
