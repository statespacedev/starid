#ifndef NOMAD_H
#define NOMAD_H

#include "Eigen/Core"
#include "star_pairs.h"
#include "settler_triangle.h"
#include "nomad_triangle.h"
using namespace Eigen;
using namespace starid;
using cands = std::set<int>;

namespace starid {
    class NOMAD {
    public:
        explicit NOMAD(Starpairs&);
        cands run(MatrixXd&);
    private:
        Starpairs starpairs;
        MatrixXd starvecs;
        std::vector<StartriangleNOMAD> tri;
    };
}

#endif
