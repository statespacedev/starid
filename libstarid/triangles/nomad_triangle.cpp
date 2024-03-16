#include "nomad_triangle.h"

starid::StartriangleNOMAD::StartriangleNOMAD() {}

void starid::StartriangleNOMAD::first(Eigen::MatrixXd &starvecs, starid::Starpairs &starpairs) {
    stara = 0; starb = 2; starc = 3;
    sv1 << 0., 0., 1.;
    sv2 = starvecs.row(starb);
    sv3 = starvecs.row(starc);
    side1 = starid::Startriangleside2(sv1, sv2, starpairs);
    stara = 0;
//    side1 = starid::Startriangleside(starid::pairangle(starvecs, stara, starb), starpairs);
//    side2 = starid::Startriangleside(starid::pairangle(starvecs, starb, starc), starpairs);
//    side3 = starid::Startriangleside(starid::pairangle(starvecs, starc, stara), starpairs);
//    this->constrain();
}

void starid::StartriangleNOMAD::from_parent() {
//    while (starc == stara || starc == starb) starc = rand() % starvecs.rows();
//    side2 = starid::Startriangleside(starid::pairangle(starvecs, starb, starc), starpairs);
//    side3 = starid::Startriangleside(starid::pairangle(starvecs, starc, stara), starpairs);
//    this->constrain();
}

void starid::StartriangleNOMAD::constrain() {
//    for (auto STAR1side1 = side1.stars.begin(), end = side1.stars.end(); STAR1side1 != end; ++STAR1side1) {
//        auto STAR1side3 = side3.stars.find(STAR1side1->first);
//        if (STAR1side3 == side3.stars.end()) continue;
//        auto &pairs3 = STAR1side3->second;
//        auto &pairs1 = STAR1side1->second;
//        for (auto pairs1it = pairs1.begin(), end = pairs1.end(); pairs1it != end; ++pairs1it) {
//            auto STAR2side2 = side2.stars.find(pairs1it->first);
//            if (STAR2side2 == side2.stars.end()) continue;
//            auto &pairs2 = STAR2side2->second;
//            for (auto pairs2it = pairs2.begin(), end = pairs2.end(); pairs2it != end; ++pairs2it) {
//                auto STAR3side3 = pairs3.find(pairs2it->first);
//                if (STAR3side3 == pairs3.end()) continue;
//                pairs1it->second = 1;
//                pairs2it->second = 1;
//                STAR3side3->second = 1;
//            }
//        }
//    } // keep these pairs
//    side1.drops();
//    side2.drops();
//    side3.drops();
}

void starid::StartriangleNOMAD::feedback(starid::Startriangleside &nextside1) {
//    side2.stars = nextside1.stars;
//    this->constrain();
}

bool starid::StartriangleNOMAD::stop() {
//    if (s12.stars.size() <= 1) return true;
    return false;
}
