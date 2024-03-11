#ifndef NOMAD_H
#define NOMAD_H

#include "Eigen/Core"
#include "star_pairs.h"
#include "settler_triangle.h"
#include "nomad_triangle.h"

namespace starid {

    class NOMAD {
    public:
        NOMAD();

        int run(Eigen::MatrixXd &pixels, Starpairs &starpairs);

    private:
        Starpairs starpairs;
        Eigen::MatrixXd starvecs;
        std::vector<StartriangleNOMAD> triangles;
    };

}

#endif
