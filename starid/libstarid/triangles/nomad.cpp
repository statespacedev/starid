#include "nomad.h"
#include "nomad_triangle.h"

NOMAD::NOMAD(Starpairs& starpairs) : starpairs(starpairs) {}

cands NOMAD::run(MatrixXd& pixels) {
    starvecs = pixels_to_starvecs(pixels);
    tri.emplace_back(StartriangleNOMAD(starvecs, starpairs));
    tri[0].first();
    while (!tri[0].stop()) {
        tri.emplace_back(StartriangleNOMAD(starvecs, starpairs));
        auto n = tri.size();
        tri[n-1].from_parent(tri[n-2].side2, tri[n-2].starb, tri[n-2].starc);
        for (auto i = n; i > 1; --i) tri[i-2].chk2(tri[i-1]);}
    cands result = tri[0].acands;
    return result;
}

