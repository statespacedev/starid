#include "settler.h"
#include "../sky/geometry.h"

SETTLER::SETTLER(Starpairs &starpairs) : starpairs(starpairs), sva{0., 0., 1.} {}

int SETTLER::run(MatrixXd &pixels) {
    starvecs = pixels_to_starvecs(pixels);
    for (Vector3d svb : starvecs.rowwise()) {
        Startriangleside2 abside(sva, svb, starpairs);
        if (!acands.empty()) abside.update_acands(acands.back());
        for (Vector3d svc : starvecs.rowwise()) {
            if (svb == svc) continue;
    }
    return -1;
}
