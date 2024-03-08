#include "nomad.h"
#include "nomad_triangle.h"

starid::NOMAD::NOMAD(Starpairs &starpairs) : starpairs(starpairs), maxtriangles(90) {}

int starid::NOMAD::run(Eigen::MatrixXd &pixels) {
    starvecs = pixels_to_starvecs(pixels);

    triangles.emplace_back(StartriangleNOMAD());
    triangles[-1].first(starvecs, starpairs);

//    int cnt = 1;
//    while (cnt < maxtriangles && !triangles[0].stop()) {
//        triangles.emplace_back(
//                StartriangleNOMAD(triangles.back().side2, triangles.back().starb,
//                                  triangles.back().starc, starvecs, starpairs));
//        for (int ndx = (int) triangles.size() - 1; ndx > 0; --ndx)
//            triangles[ndx - 1].feedback(triangles[ndx].side1);
//        ++cnt;
//    }
    return -1;
}

