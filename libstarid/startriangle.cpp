#include "startriangle.h"

/*
 * class StartriangleNOMAD:
 *    '''NOMAD triangle. focus is on the basestar and baseside - nomad is about a chain of basesides, each increasing the constraints on the preceding basestars. closing the loop around a triangle for each baseside speeds up that process.'''
 * */
starid::StartriangleNOMAD::StartriangleNOMAD(int basestar, Eigen::MatrixXd &starvecs)
    : basestar(basestar), starb(0), starc(0) {
    return;
}

/*
 * class StartriangleSETTLER:
 *    '''SETTLER triangle. acts as the triangles abca and abda within the star triangle identifier inner loops. their are three triangle sides - representing three star pairs, each with an angular separation. each side is acted by a star triangle side object.'''
 * */
starid::StartriangleSETTLER::StartriangleSETTLER(double ang1, double ang2, double ang3, double tolerance, starid::Starpairs &pairs, Eigen::Vector3d vecin)
    : side1(ang1, tolerance, pairs), side2(ang2, tolerance, pairs), side3(ang3, tolerance, pairs), tolerance(tolerance), vecstar3(vecin) {
    vecstar3 << vecin(0), vecin(1), vecin(2);
}

/*
 *    def close_loops_abda(self, triangles):
 *       '''test candidate star pairs for the sides of an abda triangle.'''
 * */
void starid::StartriangleSETTLER::close_loops_abda(std::vector<StartriangleSETTLER> &triangles, starid::Starpairs &pairs) {
    int maxtriangles = triangles.size();
    for (int trianglendx = 0; trianglendx < maxtriangles; ++trianglendx) {
        double cdang = acos(vecstar3.transpose() * triangles[trianglendx].vecstar3);
        starid::Startriangleside cd(cdang, tolerance, pairs);
        loops_cnt = 0;
        for (auto it11 = side1.stars.begin(), end = side1.stars.end(); it11 != end; ++it11) {
            auto &pairs1 = it11->second;
            int star1side1 = it11->first;                     // star1 side1
            auto star1side3 = side3.stars.find(star1side1);   // star1 side3
            if (star1side3 == side3.stars.end()) continue;
            auto &pairs3 = star1side3->second;
            for (auto pairs1it = pairs1.begin(), end = pairs1.end(); pairs1it != end; ++pairs1it) {
                int star2side1 = pairs1it->first;               // star2 side1
                auto star2side2 = side2.stars.find(star2side1); // star2 side2
                if (star2side2 == side2.stars.end()) continue;
                auto &pairs2 = star2side2->second;
                for (auto pairs2it = pairs2.begin(), end = pairs2.end(); pairs2it != end; ++pairs2it) {
                    int star3side2 = pairs2it->first;             // star3 side2
                    auto star3side3 = pairs3.find(star3side2);    // star3 side3
                    if (star3side3 == pairs3.end()) continue;
                    bool dok = false; // is this d star possible
                    auto cdd = cd.stars.find(star3side2);
                    auto aca = triangles[trianglendx].side3.stars.find(star1side1);
                    if (cdd != cd.stars.end() && aca != triangles[trianglendx].side3.stars.end()) {
                        auto &pairscdc = cdd->second;
                        auto &pairsacc = aca->second;
                        for (auto pairscdcit = pairscdc.begin(), end = pairscdc.end();
                             pairscdcit != end; ++pairscdcit) {
                            auto pairsaccit = pairsacc.find(pairscdcit->first);
                            if (pairsaccit != pairsacc.end()) {
                                dok = true;
                                break; } } }
                    if (!dok) continue;
                    pairs1it->second = 1;
                    pairs2it->second = 1;
                    star3side3->second = 1;
                    ++loops_cnt; } } }
        side1.trim_pairs(); side2.trim_pairs(); side3.trim_pairs();
        if (side1.stars.size() == 1) break; } }

/*
 *    def close_loops_abca(self):
 *       '''test candidate star pairs for the sides of an abca triangle.'''
 * */
void starid::StartriangleSETTLER::close_loops_abca() {
    loops_cnt = 0;
    for (auto it11 = side1.stars.begin(), end = side1.stars.end(); it11 != end; ++it11) {
        auto &pairs1 = it11->second;
        int star1side1 = it11->first;                     // star1 side1
        auto star1side3 = side3.stars.find(star1side1);   // star1 side3
        if (star1side3 == side3.stars.end()) continue;
        auto &pairs3 = star1side3->second;
        for (auto pairs1it = pairs1.begin(), end = pairs1.end(); pairs1it != end; ++pairs1it) {
            int star2side1 = pairs1it->first;               // star2 side1
            auto star2side2 = side2.stars.find(star2side1); // star2 side2
            if (star2side2 == side2.stars.end()) continue;
            auto &pairs2 = star2side2->second;
            for (auto pairs2it = pairs2.begin(), end = pairs2.end(); pairs2it != end; ++pairs2it) {
                int star3side2 = pairs2it->first;             // star3 side2
                auto star3side3 = pairs3.find(star3side2);    // star3 side3
                if (star3side3 == pairs3.end()) continue;
                pairs1it->second = 1;
                pairs2it->second = 1;
                star3side3->second = 1;
                ++loops_cnt; } } }
    side1.trim_pairs(); side2.trim_pairs(); side3.trim_pairs(); }