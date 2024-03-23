#ifndef STARID_NOMAD_TRIANGLE_H
#define STARID_NOMAD_TRIANGLE_H

#include "Eigen/Core"
#include "../sky/geometry.h"
#include "star_triangle_side.h"
using namespace Eigen;
using namespace starid;

namespace starid {

    class StartriangleNOMAD {
    public:
        StartriangleNOMAD(MatrixXd &starvecs, Starpairs &starpairs);

        void first();

        void from_parent();

        void feedback(Startriangleside &next);

        void chk1();

        bool stop();
    private:
        MatrixXd starvecs;
        Starpairs starpairs;
        Startriangleside2 side1, side2, side3;
        int stara, starb, starc;
        Vector3d sv1, sv2, sv3;

    };

}
#endif //STARID_NOMAD_TRIANGLE_H
