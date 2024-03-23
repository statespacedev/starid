#include "nomad.h"
#include "nomad_triangle.h"

starid::NOMAD::NOMAD(Starpairs &starpairs) : starpairs(starpairs) {}

int starid::NOMAD::run(Eigen::MatrixXd &pixels) {
    starvecs = pixels_to_starvecs(pixels);

    triangles.emplace_back(StartriangleNOMAD(starvecs, starpairs));
    triangles[0].first();

    return -1;
}

