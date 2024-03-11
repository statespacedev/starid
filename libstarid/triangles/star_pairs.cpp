#include "star_pairs.h"

starid::Starpairs::Starpairs() {}

void starid::Starpairs::generate(starid::Sky &sky) {
    int pairndx = 0;
    for (auto star: sky.stars) {
        std::vector<int> starndxs = sky.stars_near_point(star.x, star.y, star.z);
        starndxs.push_back(star.starndx);
        for (auto starndx1: starndxs) {
            for (auto starndx2: starndxs) {
                if (starndx1 == starndx2) continue;
                std::string key = pair_labeler(starndx1, starndx2);
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

std::unordered_map<int, std::unordered_map<int, int>>
starid::Starpairs::pairs_for_angle(double angle, double tol_radius) {
    std::unordered_map<int, std::unordered_map<int, int>> stars;
    double ang1 = angle - tol_radius;
    double ang2 = angle + tol_radius;
    double epsilon = 1.0;
    if (ang1 <= 0) ang1 = 0;
    if (ang2 <= epsilon * tol_radius) ang2 = epsilon * tol_radius;
    if (ang1 >= starid::star_pair_angle_limit - epsilon * tol_radius)
        ang1 = starid::star_pair_angle_limit - epsilon * tol_radius;
    if (ang2 >= starid::star_pair_angle_limit) ang2 = starid::star_pair_angle_limit;
    std::vector<int> pairndxs = angndxs.findndxs(ang1, ang2);
    for (auto ndx: pairndxs) {
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

std::string starid::Starpairs::pair_labeler(int catndx1, int catndx2) {
    if (catndx1 > catndx2) {
        int tmp = catndx1;
        catndx1 = catndx2;
        catndx2 = tmp;
    }
    std::string label = std::to_string(catndx1) + std::to_string(catndx2);
    return label;
}


