#ifndef NOMAD_H
#define NOMAD_H
#include "include/Eigen/Core"
#include "triangles/starpairs.h"
#include "triangles/startriangle.h"

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

}

#endif
