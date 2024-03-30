#include "settler.h"
#include "../sky/geometry.h"

SETTLER::SETTLER(Starpairs &starpairs) : starpairs{starpairs}, sva{0., 0., 1.} {}

cands SETTLER::run(MatrixXd &pixels) {
    starvecs = pixels_to_starvecs(pixels);
    for (Vector3d svb : starvecs.rowwise()) {
        Startriangleside2 abside(sva, svb, starpairs);
        bool bypass {false};
        if (!acands.empty()) abside.update_acands(acands.back());
        for (Vector3d svc : starvecs.rowwise()) {
            if (bypass || svb == svc) continue;
            std::vector<StartriangleSETTLER> tri {StartriangleSETTLER(svb, svc, starpairs, abside)};
            tri[0].chk1();
            for (Vector3d svd : starvecs.rowwise()) {
                if (bypass || svd == svc || svd == svb) continue;
                auto tmp = StartriangleSETTLER(svb, svd, starpairs, abside);
                tmp.chk2(tri);
                tri.push_back(tmp);
                if (abside.starcnt.end()[-1] == abside.starcnt.end()[-2]) bypass = true;}}
        acands.push_back(abside.getcands());
        if (acands.back().size() <= 1) break;}
    cands result = acands.end()[-1];
    return result;
}
