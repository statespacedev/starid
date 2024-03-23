#include <set>
#include "nomad_triangle.h"

StartriangleNOMAD::StartriangleNOMAD(MatrixXd &starvecs, Starpairs &starpairs) :
    stara{0}, starb{0}, starc{0}, starvecs(starvecs), starpairs(starpairs) {
    sv1 << 0., 0., 0.;
    sv2 << 0., 0., 0.;
    sv3 << 0., 0., 0.;
}

void StartriangleNOMAD::first() {
    stara = 0; starb = 2; starc = 3;
    sv1 << 0., 0., 1.;
    sv2 = starvecs.row(starb);
    sv3 = starvecs.row(starc);
    side1 = Startriangleside2(sv1, sv2, starpairs);
    side2 = Startriangleside2(sv2, sv3, starpairs);
    side3 = Startriangleside2(sv3, sv1, starpairs);
    this->chk1();
}

void StartriangleNOMAD::from_parent() {
//    while (starc == stara || starc == starb) starc = rand() % starvecs.rows();
//    side2 = Startriangleside(pairangle(starvecs, starb, starc), starpairs);
//    side3 = Startriangleside(pairangle(starvecs, starc, stara), starpairs);
//    this->constrain();
}

void StartriangleNOMAD::chk1() {
    std::set<int> ok1, ok2, ok3;
    for (auto & a1 : side1.stars) {
        if (!side3.stars.contains(a1.first)) continue;
        for (auto & b1 : a1.second) {
            if (!side2.stars.contains(b1)) continue;
            for (auto & c2 : side2.stars[b1]) {
                if (!side3.stars[a1.first].contains(c2)) continue;
                ok1.insert(a1.first); ok1.insert(b1);
                ok2.insert(b1); ok2.insert(c2);
                ok3.insert(c2); ok3.insert(a1.first);
            }
        }
    }
    side1.update_side(ok1);
    side2.update_side(ok2);
    side3.update_side(ok3);
}

void StartriangleNOMAD::feedback(Startriangleside &nextside1) {
//    side2.stars = nextside1.stars;
//    this->constrain();
}

bool StartriangleNOMAD::stop() {
//    if (s12.stars.size() <= 1) return true;
    return false;
}
