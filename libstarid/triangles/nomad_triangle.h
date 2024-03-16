#ifndef STARID_NOMAD_TRIANGLE_H
#define STARID_NOMAD_TRIANGLE_H

#include "Eigen/Core"
#include "../sky/geometry.h"
#include "star_triangle_side.h"

namespace starid {

    class StartriangleNOMAD {
    public:
        StartriangleNOMAD();

        void first(Eigen::MatrixXd &starvecs, Starpairs &starpairs);

        void from_parent();

        void feedback(starid::Startriangleside &next);

        void constrain();

        bool stop();
    private:
//        Eigen::MatrixXd starvecs;
        Starpairs starpairs;
        Startriangleside2 side1;
        int stara, starb, starc;
        Eigen::Vector3d sv1, sv2, sv3;

    };

}
#endif //STARID_NOMAD_TRIANGLE_H
