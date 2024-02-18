#include "nomad.h"
#include "../sky/geometry.h"

/*
 * class NOMAD:
 *    '''star recognition focused on a chain of triangles and basesides - side2 of each triangle is the baseside of the following triangle. during feedback, these shared side2 -> baseside pairs are the path for information to flow backwards - increasing the constraints on the initial triangle baseside and basestar. the name NOMAD relates to how the chain of triangles wanders away from the target star and initial triangle.'''
 * */
starid::NOMAD::NOMAD(Starpairs &starpairs) : starpairs(starpairs), maxtriangles(90) {}

/*
 *    def run(self, pixels):
 *       '''recognize target star from the image pixels.'''
 * */
int starid::NOMAD::run(Eigen::MatrixXd &pixels) {
    starvecs = pixels_to_starvecs(pixels);
    triangles.emplace_back(StartriangleNOMAD(0, starvecs, starpairs)); int cnt = 1;
    while (cnt < maxtriangles && !triangles[0].stop()) {
        triangles.emplace_back(StartriangleNOMAD(triangles.back().side2, triangles.back().starb, triangles.back().starc, starvecs, starpairs));
        for (int ndx = (int)triangles.size() - 1; ndx > 0; --ndx) triangles[ndx - 1].feedback(triangles[ndx].side1);
        ++cnt; }
    return -1;
}

