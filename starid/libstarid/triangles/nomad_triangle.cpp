#include "nomad_triangle.h"

StartriangleNOMAD::StartriangleNOMAD(MatrixXd &starvecs, Starpairs &starpairs) :
    stara{0}, starb{0}, starc{0}, starvecs(starvecs), starpairs(starpairs) {
    sv1 << 0., 0., 0.; sv2 << 0., 0., 0.; sv3 << 0., 0., 0.;
}

void StartriangleNOMAD::first() {
    stara = 0; starb = 1; starc = 2;
    sv1 << 0., 0., 1.; sv2 = starvecs.row(starb); sv3 = starvecs.row(starc);
    side1 = Startriangleside2(sv1, sv2, starpairs);
    side2 = Startriangleside2(sv2, sv3, starpairs);
    side3 = Startriangleside2(sv3, sv1, starpairs);
    this->chk1();
}

void StartriangleNOMAD::from_parent(Startriangleside2& side2_, int starb_, int starc_) {
    stara = starb_; starb = starc_; starc = starc_;
    while (starc == stara || starc == starb) starc += 1;
    sv1 = starvecs.row(stara); sv2 = starvecs.row(starb); sv3 = starvecs.row(starc);
    side1 = side2_;
    side2 = Startriangleside2(sv2, sv3, starpairs);
    side3 = Startriangleside2(sv3, sv1, starpairs);
    this->chk1();
}

void StartriangleNOMAD::chk1() {
    cands ok1, ok2, ok3;
    for (auto& a1 : side1.stars) {
        if (!side3.stars.contains(a1.first)) continue;
        for (auto& b1 : a1.second) {
            if (!side2.stars.contains(b1)) continue;
            for (auto& c2 : side2.stars[b1]) {
                if (!side3.stars[a1.first].contains(c2)) continue;
                ok1.insert(a1.first); ok1.insert(b1); ok2.insert(b1); ok2.insert(c2); ok3.insert(c2); ok3.insert(a1.first);}}}
    side1.update_side(ok1); side2.update_side(ok2); side3.update_side(ok3);}

void StartriangleNOMAD::chk2(StartriangleNOMAD& other) {
    auto adside = Startriangleside2(sv1, other.sv3, starpairs, .00685);
    cands ok1, ok2, ok3, tmp;
    for (auto& a1 : side1.stars) {
        if (!side3.stars.contains(a1.first)) continue;
        for (auto& b1 : a1.second) {
            if (!side2.stars.contains(b1)) continue;
            for (auto& c2 : side2.stars[b1]) {
                if (!side3.stars[a1.first].contains(c2)) continue;
                if (!adside.stars.contains(c2)) continue;
                if (!adside.stars.contains(a1.first) || !other.side2.stars.contains(c2)) continue;
                tmp.clear();
                std::set_intersection(adside.stars[a1.first].begin(), adside.stars[a1.first].end(),
                                      other.side2.stars[c2].begin(), other.side2.stars[c2].end(), std::inserter(tmp, tmp.begin()));
                if (tmp.empty()) continue;
                ok1.insert(a1.first); ok1.insert(b1); ok2.insert(b1); ok2.insert(c2); ok3.insert(c2); ok3.insert(a1.first);}}}
    side1.update_side(ok1); side2.update_side(ok2); side3.update_side(ok3);}

bool StartriangleNOMAD::stop() {
    cands c1, c3, tmp;
    for (auto& a1 : side1.stars) c1.insert(a1.first);
    for (auto& a3 : side3.stars) c3.insert(a3.first);
    std::set_intersection(c1.begin(), c1.end(), c3.begin(), c3.end(), std::inserter(tmp, tmp.begin()));
    acands = tmp;
    if (acands.size() > 1) return false;
    return true;
}
