#ifndef STARID_SETTLER_TRIANGLE_H
#define STARID_SETTLER_TRIANGLE_H
#include "star_triangle_side.h"
using namespace starid;
using namespace Eigen;
using cands = std::set<int>;

namespace starid {
    class StartriangleSETTLER {
    public:
        StartriangleSETTLER(Vector3d&, Vector3d&, Starpairs&, Startriangleside2&);
        void chk1();
        void chk2(std::vector<StartriangleSETTLER>&);
        Startriangleside2 side1, side2, side3;
        Startriangleside2& abside;
        Vector3d vecstar3;
    private:
        Starpairs starpairs;
        double tol;
    };
}

#endif //STARID_SETTLER_TRIANGLE_H
