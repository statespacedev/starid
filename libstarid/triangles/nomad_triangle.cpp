#include "nomad_triangle.h"

starid::StartriangleNOMAD::StartriangleNOMAD(int basestar, Eigen::MatrixXd &starvecs, starid::Starpairs &starpairs)
        : stara(basestar), starb(basestar), starc(basestar) {
    srand(time(NULL));
    while (starb == stara) starb = rand() % starvecs.rows();
    while (starc == stara || starc == starb) starc = rand() % starvecs.rows();
    side1 = starid::Startriangleside(starid::pairangle(starvecs, stara, starb), starpairs);
    side2 = starid::Startriangleside(starid::pairangle(starvecs, starb, starc), starpairs);
    side3 = starid::Startriangleside(starid::pairangle(starvecs, starc, stara), starpairs);
    this->constrain();
}

starid::StartriangleNOMAD::StartriangleNOMAD(starid::Startriangleside &prevside2, int prevstarb, int prevstarc,
                                             Eigen::MatrixXd &starvecs, starid::Starpairs &starpairs)
        : side1(prevside2), stara(prevstarb), starb(prevstarc), starc(prevstarc) {
    while (starc == stara || starc == starb) starc = rand() % starvecs.rows();
    side2 = starid::Startriangleside(starid::pairangle(starvecs, starb, starc), starpairs);
    side3 = starid::Startriangleside(starid::pairangle(starvecs, starc, stara), starpairs);
    this->constrain();
}

void starid::StartriangleNOMAD::constrain() {
    for (auto STAR1side1 = side1.stars.begin(), end = side1.stars.end(); STAR1side1 != end; ++STAR1side1) {
        auto STAR1side3 = side3.stars.find(STAR1side1->first);
        if (STAR1side3 == side3.stars.end()) continue;
        auto &pairs3 = STAR1side3->second;
        auto &pairs1 = STAR1side1->second;
        for (auto pairs1it = pairs1.begin(), end = pairs1.end(); pairs1it != end; ++pairs1it) {
            auto STAR2side2 = side2.stars.find(pairs1it->first);
            if (STAR2side2 == side2.stars.end()) continue;
            auto &pairs2 = STAR2side2->second;
            for (auto pairs2it = pairs2.begin(), end = pairs2.end(); pairs2it != end; ++pairs2it) {
                auto STAR3side3 = pairs3.find(pairs2it->first);
                if (STAR3side3 == pairs3.end()) continue;
                pairs1it->second = 1;
                pairs2it->second = 1;
                STAR3side3->second = 1;
            }
        }
    } // keep these pairs
    side1.drops();
    side2.drops();
    side3.drops();
}

void starid::StartriangleNOMAD::feedback(starid::Startriangleside &nextside1) {
    side2.stars = nextside1.stars;
    this->constrain();
}

bool starid::StartriangleNOMAD::stop() {
    if (side1.stars.size() <= 1) return true;
    return false;
}
