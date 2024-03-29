#ifndef SETTLER_H
#define SETTLER_H

#include "Eigen/Core"
#include "star_pairs.h"
#include "settler_triangle.h"
using namespace starid;
using namespace Eigen;
using cands = std::set<int>;

namespace starid {
    class SETTLER {
    public:
        explicit SETTLER(Starpairs &pairs);
        int run(MatrixXd &pixels);
    private:
        Starpairs starpairs;
        MatrixXd starvecs;
        Vector3d sva;
        std::vector<std::set<int>> acands;
    };
}

#endif
