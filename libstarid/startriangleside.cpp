#include "startriangleside.h"

/*
 * class Startriangleside:
 *    '''act as one of the three triangle sides within a parent star triangle object. here stars is a representation of candidate star pairs that could belong to the side. ultimately - when we've recognized the target star, all but one candidate star pair is eliminated.'''
 * */
starid::Startriangleside::Startriangleside(double ang, starid::Starpairs &pairs) {
    angtol = 2.0 * std::sqrt(500.0 * 500.0 + 500.00 * 500.0) * arcseconds_to_radians;
    stars = pairs.pairs_for_angle(ang, angtol);
    log_star_count.push_back(stars.size());
    log_pair_count.push_back(countpairs());
}

starid::Startriangleside::Startriangleside() {}

/*
 *    def update(self, side):
 *       '''used just for the abside currently being investigated, to update it based on the latest abca or abda triangle.'''
 * */
void starid::Startriangleside::update(Startriangleside &side) {
    stars = side.stars;
    for (auto tmp : side.log_pair_count) log_pair_count.push_back(tmp);
    for (auto tmp : side.log_star_count) log_star_count.push_back(tmp); }

/*
 *    def drops(self):
 *       '''there's a pairhalf1 -> pairhalf -> 0 or 1 concept. 0 is the default and means drop this particular pair. here we drop all pairs that have not been set to 1, and reset all that remain to 0.'''
 * */
void ::starid::Startriangleside::drops(bool dolog) {
    for (auto star1 = stars.begin(), end = stars.end(); star1 != end; ++star1) {
        auto &pairs = star1->second;
        for (auto star2 = pairs.begin(); star2 != pairs.end();) {
            if (star2->second == 0) { star2 = pairs.erase(star2); }
            else { star2->second = 0; ++star2; } } }
    for (auto star1 = stars.begin(); star1 != stars.end();) {
        auto &pairs = star1->second;
        if (pairs.empty()) star1 = stars.erase(star1);
        else ++star1; }
    if (dolog) { log_star_count.push_back(stars.size()); log_pair_count.push_back(countpairs()); } }

/*
 *    def countpairs(self):
 *       '''how many candidate star pairs remain in this side.'''
 * */
int starid::Startriangleside::countpairs() {
    int result = 0;
    for (auto it1 = stars.begin(), end = stars.end(); it1 != end; ++it1) {
        result += it1->second.size(); }
    return result;
}

