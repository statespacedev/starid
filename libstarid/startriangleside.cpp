#include "startriangleside.h"

/*
 * class Startriangleside:
 *    '''act as one of the three triangle sides within a parent star triangle object. here stars is a representation of candidate star pairs that could belong to the side. ultimately - when we've recognized the target star, all but one candidate star pair is eliminated.'''
 * */
starid::Startriangleside::Startriangleside(double ang, double tolerance, starid::Starpairs &pairs, int starndx)
        : teststar(starndx) {
    stars = pairs.pairs_indexer(ang, tolerance); }

/*
 *    def update(self, side):
 *       '''used just for the abside currently being investigated, to update it based on the latest abca or abda triangle.'''
 * */
void starid::Startriangleside::update(Startriangleside &side) {
    stars = side.stars;
    for (auto tmp : side.log_pair_count) log_pair_count.push_back(tmp);
    for (auto tmp : side.log_star_count) log_star_count.push_back(tmp);
    for (auto tmp : side.log_teststar) log_teststar.push_back(tmp);
    has_teststar = side.has_teststar; }

/*
 *    def trim_pairs(self):
 *       '''eliminate candidate star pairs from the side.'''
 * */
void ::starid::Startriangleside::trim_pairs() {
    for (auto star1 = stars.begin(), end = stars.end(); star1 != end; ++star1) {
        auto &pairs = star1->second;
        for (auto star2 = pairs.begin(); star2 != pairs.end();) {
            if (star2->second == 0) { star2 = pairs.erase(star2); }
            else { star2->second = 0; ++star2; } } }
    for (auto star1 = stars.begin(); star1 != stars.end();) {
        auto &pairs = star1->second;
        if (pairs.empty()) star1 = stars.erase(star1);
        else ++star1; }
    has_teststar = check_teststar(teststar);
    log_star_count.push_back(stars.size());
    log_pair_count.push_back(pair_count());
    log_teststar.push_back(has_teststar);
}

/*
 *    def pair_count(self):
 *       '''how many candidate star pairs remain in the side.'''
 * */
int starid::Startriangleside::pair_count() {
    int result = 0;
    for (auto it1 = stars.begin(), end = stars.end(); it1 != end; ++it1) {
        result += it1->second.size(); }
    return result;
}

/*
 *    def check_teststar(self, strndx):
 *       '''is the test star in the side.'''
 * */
bool starid::Startriangleside::check_teststar(int starndx) {
    auto it = stars.find(starndx);
    if (it == stars.end()) return false;
    return true;
}

starid::Startriangleside::Startriangleside() {}
