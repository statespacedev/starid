#include <algorithm>
#include "star_triangle_side.h"

Startriangleside::Startriangleside(double ang, Starpairs& pairs) {
    angtol = 2.0 * std::sqrt(500.0 * 500.0 + 500.00 * 500.0) * arcseconds_to_radians;
    stars = pairs.pairs_for_angle(ang, angtol);
    log_star_count.push_back(stars.size());
    log_pair_count.push_back(countpairs());
}

Startriangleside::Startriangleside() {}

void Startriangleside::update(Startriangleside& side) {
    stars = side.stars;
    for (auto tmp: side.log_pair_count) log_pair_count.push_back(tmp);
    for (auto tmp: side.log_star_count) log_star_count.push_back(tmp);}

void Startriangleside::drops(bool dolog) {
    for (auto star1 = stars.begin(), end = stars.end(); star1 != end; ++star1) {
        auto &pairs = star1->second;
        for (auto star2 = pairs.begin(); star2 != pairs.end();) {
            if (star2->second == 0) { star2 = pairs.erase(star2); }
            else {
                star2->second = 0;
                ++star2;}}}
    for (auto star1 = stars.begin(); star1 != stars.end();) {
        auto &pairs = star1->second;
        if (pairs.empty()) star1 = stars.erase(star1);
        else ++star1;}
    if (dolog) {
        log_star_count.push_back(stars.size());
        log_pair_count.push_back(countpairs());}}

int Startriangleside::countpairs() {
    int result = 0;
    for (auto it1 = stars.begin(), end = stars.end(); it1 != end; ++it1) result += it1->second.size();
    return result;}

Startriangleside2::Startriangleside2(Vector3d sv1, Vector3d sv2, Starpairs& pairs, float angtol) :
    sv1{sv1}, sv2{sv2}, ang{std::acos(sv1.transpose() * sv2)}, stars{} {
    auto tmp = pairs.pairs_for_angle(ang, angtol);
    for (auto & star1 : tmp) {
        for (auto & star2 : star1.second) {
            if (!stars.contains(star1.first)) stars.insert({star1.first, {star2.first}});
            else stars[star1.first].insert(star2.first);}}
    log_star_count.push_back(stars.size());
    log_pair_count.push_back(countpairs());}

Startriangleside2::Startriangleside2() : stars{}, ang{0}, log_star_count{}, log_pair_count{} {}

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
    log_star_count.push_back(stars.size());
    log_pair_count.push_back(countpairs());}

int Startriangleside2::countpairs() {
    int result = 0;
    for (auto & star : stars) result += star.second.size();
    return result;}
