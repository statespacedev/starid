#ifndef STARID_STARTRIANGLE_H
#define STARID_STARTRIANGLE_H
#include "startriangleside.h"

namespace starid {

class StartriangleNOMAD {
public:
    StartriangleNOMAD(int basestar, Eigen::MatrixXd &, starid::Starpairs &);
    StartriangleNOMAD(starid::Startriangleside &, int prevstarb, int prevstarc, Eigen::MatrixXd &, starid::Starpairs &);
    void feedback(starid::Startriangleside &next);
    void constrain();
    bool stop();
    starid::Startriangleside side1, side2, side3;
    int stara, starb, starc;
};

class StartriangleSETTLER {
    public:
        StartriangleSETTLER(double ang1, double ang2, double ang3, starid::Starpairs &pairs, Eigen::Vector3d vecstar3);
        void constrain_abca();
        void constrain_abda(std::vector<StartriangleSETTLER> &triangles, starid::Starpairs &pairs);
        starid::Startriangleside side1, side2, side3;
        int loops_cnt{};
        Eigen::Vector3d vecstar3;
    };

}

#endif //STARID_STARTRIANGLE_H
