#ifndef STARID_NOMAD_TRIANGLE_H
#define STARID_NOMAD_TRIANGLE_H

#include "Eigen/Core"
#include "../sky/geometry.h"
#include "star_triangle_side.h"
using namespace Eigen;
using namespace starid;
using cands = std::set<int>;

namespace starid {
    class StartriangleNOMAD {
    public:
        StartriangleNOMAD(MatrixXd&, Starpairs&);
        void first();
        void from_parent(Startriangleside2&, int, int);
        void chk1();
        void chk2(StartriangleNOMAD&);
        bool stop();
        Startriangleside2 side1, side2, side3;
        int stara, starb, starc;
        cands acands;
    private:
        MatrixXd starvecs;
        Starpairs starpairs;
        Vector3d sv1, sv2, sv3;
    };
}
#endif //STARID_NOMAD_TRIANGLE_H
