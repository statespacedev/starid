#include "startriangleidentifier.h"
#include "starpairs.h"

/*
 * class Starpairs:
 *    '''foundation for star triangles - a star pair is a triangle-side - it's the fundamental particle of a triangle view of the sky. every triangle is made of three starpairs. in a starpair object, for each star, the starpairs with each of its near neighbors are represented, pre-computed and ready for use. computing this object is relatively heavy and we want to do it once, in advance, and then reuse it from there. so cerealize it to a starpairs file and read in the starpairs object from that whenever possible, rather than generating from scratch.'''
 * */
starid::Starpairs::Starpairs() {}

/*
 *    def generate(self, sky):
 *       ''' '''
 * */
void starid::Starpairs::generate(starid::Sky &sky) {
    int pairndx = 0;
    for (auto star : sky.stars) {
        std::vector<int> starndxs = sky.stars_near_point(star.x, star.y, star.z);
        starndxs.push_back(star.starndx);
        for (auto starndx1 : starndxs) {
            for (auto starndx2 : starndxs) {
                if (starndx1 == starndx2) continue;
                std::string key = pairlabeler(sky.stars[starndx1].starndx, sky.stars[starndx2].starndx);
                auto search = starpairsndxs.find(key);
                if (search != starpairsndxs.end()) continue; // check map that pair is unique
                double angle = acos((sky.stars[starndx1].x * sky.stars[starndx2].x) +
                                         (sky.stars[starndx1].y * sky.stars[starndx2].y) +
                                         (sky.stars[starndx1].z * sky.stars[starndx2].z));
                if (std::abs(angle) > starid::star_pair_angle_limit) continue; // max pair angle
                std::tuple<double, int, int> starpair{angle, starndx1, starndx2};
                starpairs.push_back(starpair);
                starpairsndxs.insert({key, pairndx}); // update map of unique pairs
                angndxs.add_pair(angle, pairndx);
                ++pairndx;
            }
        }
    }
    angndxs.sort();
}

/*
 *    def pairsndxr(self, angle, tol_radius):
 *       ''' '''
 * */
std::unordered_map<int, std::unordered_map<int, int>> starid::Starpairs::pairsndxr(double angle, double tol_radius) {
    std::unordered_map<int, std::unordered_map<int, int>> stars;
    double ang1 = angle - tol_radius;
    double ang2 = angle + tol_radius;
    double epsilon = 1.0;
    if (ang1 <= 0) ang1 = 0;
    if (ang2 <= epsilon * tol_radius) ang2 = epsilon * tol_radius;
    if (ang1 >= starid::star_pair_angle_limit - epsilon * tol_radius)
        ang1 = starid::star_pair_angle_limit - epsilon * tol_radius;
    if (ang2 >= starid::star_pair_angle_limit) ang2 = starid::star_pair_angle_limit;
    std::vector<int> intsFromTable = angndxs.findndxs(ang1, ang2);
    for (auto ndx : intsFromTable) {
        int star1 = std::get<1>(starpairs[ndx]);
        int star2 = std::get<2>(starpairs[ndx]);
        auto it1 = stars.find(star1);
        if (it1 != stars.end()) {
            auto &pairs1 = it1->second;
            pairs1.emplace(std::make_pair(star2, 0)); // initial value for pair star key is 0
        } else {
            std::unordered_map<int, int> pairs1;
            pairs1.emplace(std::make_pair(star2, 1));
            stars.emplace(std::make_pair(star1, pairs1));
        }
        auto it2 = stars.find(star2);
        if (it2 != stars.end()) {
            auto &pairs2 = it2->second;
            pairs2.emplace(std::make_pair(star1, 0)); // initial value for pair star key is 0
        } else {
            std::unordered_map<int, int> pairs2;
            pairs2.emplace(std::make_pair(star1, 1));
            stars.emplace(std::make_pair(star2, pairs2));
        }
    }
    return stars;
}

/*
 *    def pairlabeler(self, catndx1, catndx2):
 *       ''' '''
 * */
std::string starid::Starpairs::pairlabeler(int catndx1, int catndx2) {
    if (catndx1 > catndx2) {
        int tmp = catndx1;
        catndx1 = catndx2;
        catndx2 = tmp;
    }
    std::string label = std::to_string(catndx1) + std::to_string(catndx2);
    return label;
}