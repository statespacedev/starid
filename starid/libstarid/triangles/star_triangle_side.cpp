#include <algorithm>
#include "star_triangle_side.h"

Startriangleside2::Startriangleside2(Vector3d sv1, Vector3d sv2, Starpairs& pairs, float angtol) :
    sv1{sv1}, sv2{sv2}, ang{std::acos(sv1.transpose() * sv2)}, stars{} {
    stars = pairs.pairs_for_angle(ang, angtol);
    starcnt.push_back(stars.size());
    paircnt.push_back(countpairs());}

Startriangleside2::Startriangleside2() : stars{}, ang{0}, starcnt{}, paircnt{} {}

void Startriangleside2::update_side(std::set<int>& ok) {
    std::set<int> drops;
    for (auto & star : stars) {
        if (!ok.contains(star.first)) drops.insert(star.first);
        else {
            std::set<int> tmp;
            std::set_intersection(ok.begin(), ok.end(), star.second.begin(), star.second.end(), std::inserter(tmp, tmp.begin()));
            star.second = tmp;
            if (star.second.size() == 0) drops.insert(star.first);}}
    for (auto & star : drops) stars.erase(star);
    starcnt.push_back(stars.size());
    paircnt.push_back(countpairs());}

void Startriangleside2::update_abside(Startriangleside2& side) {
    starsdict2 tmp;
    for (auto& star : side.stars) {
        if (!stars.contains(star.first)) continue;
        std::set<int> tmp2;
        std::set_intersection(stars[star.first].begin(), stars[star.first].end(),
                              star.second.begin(), star.second.end(), std::inserter(tmp2, tmp2.begin()));
        if (tmp2.empty()) continue;
        tmp[star.first] = tmp2;}
    stars = tmp;
    starcnt.push_back(stars.size());
    paircnt.push_back(countpairs());}

void Startriangleside2::update_acands(cands& acands) {
    std::vector<int> drops;
    for (auto star : stars) {
        if (!acands.contains(star.first)) drops.push_back(star.first);}
    for (auto star : drops) stars.erase(star);
    starcnt.push_back(stars.size());
    paircnt.push_back(countpairs());}

cands Startriangleside2::getcands() {
    cands cur;
    for (auto star : stars) cur.insert(star.first);
    return cur;}

int Startriangleside2::countpairs() {
    int result = 0;
    for (auto & star : stars) result += star.second.size();
    return result;}
