#include "settler_triangle.h"
//#include "../sky/geometry.h"
//#include "nomad_triangle.h"

starid::StartriangleSETTLER::StartriangleSETTLER(double ang1, double ang2, double ang3, starid::Starpairs &pairs,
                                                 Eigen::Vector3d vecin)
        : side1(ang1, pairs), side2(ang2, pairs), side3(ang3, pairs), vecstar3(vecin) {
    vecstar3 << vecin(0), vecin(1), vecin(2);
}

void
starid::StartriangleSETTLER::constrain_abda(std::vector<StartriangleSETTLER> &triangles, starid::Starpairs &pairs) {
    int maxtriangles = triangles.size();
    for (int trianglendx = 0; trianglendx < maxtriangles; ++trianglendx) {
        double cdang = acos(vecstar3.transpose() * triangles[trianglendx].vecstar3);
        starid::Startriangleside cd(cdang, pairs);
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
                                break;
                            }
                        }
                    }
                    if (!dok) continue;
                    pairs1it->second = 1;
                    pairs2it->second = 1;
                    star3side3->second = 1;
                    ++loops_cnt;
                }
            }
        }
        side1.drops();
        side2.drops();
        side3.drops();
        if (side1.stars.size() == 1) break;
    }
}

void starid::StartriangleSETTLER::constrain_abca() {
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
                ++loops_cnt;
            }
        }
    }
    side1.drops();
    side2.drops();
    side3.drops();
}