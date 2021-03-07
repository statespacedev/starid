#ifndef STARTRIANGLES_H
#define STARTRIANGLES_H
#include <Eigen/Core>
#include "starpairs.h"
#include "startriangle.h"

namespace starid {

class NOMAD {
public:
    NOMAD(Starpairs &pairs);
    int run(Eigen::MatrixXd &pixels);
private:
    Starpairs starpairs;
    Eigen::MatrixXd starvecs;
    std::vector<StartriangleNOMAD> triangles;
    int maxtriangles;
};

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
