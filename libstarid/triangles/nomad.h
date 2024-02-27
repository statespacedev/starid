#ifndef NOMAD_H
#define NOMAD_H
#include "include/Eigen/Core"
#include "triangles/star_pairs.h"
#include "triangles/settler_triangle.h"

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
