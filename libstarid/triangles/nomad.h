#ifndef NOMAD_H
#define NOMAD_H

#include "Eigen/Core"
#include "star_pairs.h"
#include "settler_triangle.h"
#include "nomad_triangle.h"

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
