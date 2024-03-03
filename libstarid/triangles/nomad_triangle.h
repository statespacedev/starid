#ifndef STARID_NOMAD_TRIANGLE_H
#define STARID_NOMAD_TRIANGLE_H

#include "../sky/geometry.h"
#include "star_triangle_side.h"

namespace starid {

    class StartriangleNOMAD {
    public:
        StartriangleNOMAD(int basestar, Eigen::MatrixXd &, starid::Starpairs &);

        StartriangleNOMAD(starid::Startriangleside &, int prevstarb, int prevstarc, Eigen::MatrixXd &,
                          starid::Starpairs &);

        void feedback(starid::Startriangleside &next);

        void constrain();

        bool stop();

        starid::Startriangleside side1, side2, side3;
        int stara, starb, starc;
    };

}
#endif //STARID_NOMAD_TRIANGLE_H
