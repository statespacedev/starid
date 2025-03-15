#include "settler_triangle.h"
//#include "../sky/geometry.h"
//#include "nomad_triangle.h"

StartriangleSETTLER::StartriangleSETTLER(Vector3d& sv2, Vector3d& sv3, Starpairs& starpairs, Startriangleside2& abside)
        : starpairs{starpairs}, abside{abside}, vecstar3{sv3} {
    tol = .003;
    Vector3d sv1 {0., 0., 1.};
    side1 = Startriangleside2(sv1, sv2, starpairs, tol);
    side2 = Startriangleside2(sv2, sv3, starpairs, tol);
    side3 = Startriangleside2(sv3, sv1, starpairs, tol);
    side1.stars = abside.stars;}

void StartriangleSETTLER::chk1() {
    cands ok1, ok2, ok3;
    for (auto& a1 : side1.stars) {
        if (!side3.stars.contains(a1.first)) continue;
        for (auto& b1 : a1.second) {
            if (!side2.stars.contains(b1)) continue;
            for (auto& c2 : side2.stars[b1]) {
                if (!side3.stars[a1.first].contains(c2)) continue;
                ok1.insert(a1.first); ok1.insert(b1); ok2.insert(b1); ok2.insert(c2); ok3.insert(c2); ok3.insert(a1.first);}}}
    side1.update_side(ok1); side2.update_side(ok2); side3.update_side(ok3);
    abside.update_abside(side1);}

void StartriangleSETTLER::chk2(std::vector<StartriangleSETTLER>& tri) {
    for (auto& other : tri) {
        auto cdside = Startriangleside2(vecstar3, other.vecstar3, starpairs, tol);
        cands ok1, ok2, ok3, tmp;
        for (auto& a1 : side1.stars) {
            if (!side3.stars.contains(a1.first)) continue;
            for (auto& b1 : a1.second) {
                if (!side2.stars.contains(b1)) continue;
                for (auto& c2 : side2.stars[b1]) {
                    if (!side3.stars[a1.first].contains(c2)) continue;
                    if (!cdside.stars.contains(c2)) continue;
                    if (!other.side3.stars.contains(a1.first)) continue;
                    tmp.clear();
                    std::set_intersection(cdside.stars[c2].begin(), cdside.stars[c2].end(),
                                          other.side3.stars[a1.first].begin(), other.side3.stars[a1.first].end(),
                                          std::inserter(tmp, tmp.begin()));
                    if (tmp.empty()) continue;
                    ok1.insert(a1.first); ok1.insert(b1); ok2.insert(b1); ok2.insert(c2); ok3.insert(c2); ok3.insert(a1.first);}}}
        side1.update_side(ok1); side2.update_side(ok2); side3.update_side(ok3);
    }
    abside.update_abside(side1);
}
