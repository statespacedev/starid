#include "star_triangle_side.h"

starid::Startriangleside::Startriangleside(double ang, starid::Starpairs &pairs) {
    angtol = 2.0 * std::sqrt(500.0 * 500.0 + 500.00 * 500.0) * arcseconds_to_radians;
    stars = pairs.pairs_for_angle(ang, angtol);
    log_star_count.push_back(stars.size());
    log_pair_count.push_back(countpairs());
}

starid::Startriangleside::Startriangleside() {}

void starid::Startriangleside::update(Startriangleside &side) {
    stars = side.stars;
    for (auto tmp: side.log_pair_count) log_pair_count.push_back(tmp);
    for (auto tmp: side.log_star_count) log_star_count.push_back(tmp);
}

void ::starid::Startriangleside::drops(bool dolog) {
    for (auto star1 = stars.begin(), end = stars.end(); star1 != end; ++star1) {
        auto &pairs = star1->second;
        for (auto star2 = pairs.begin(); star2 != pairs.end();) {
            if (star2->second == 0) { star2 = pairs.erase(star2); }
            else {
                star2->second = 0;
                ++star2;
            }
        }
    }
    for (auto star1 = stars.begin(); star1 != stars.end();) {
        auto &pairs = star1->second;
        if (pairs.empty()) star1 = stars.erase(star1);
        else ++star1;
    }
    if (dolog) {
        log_star_count.push_back(stars.size());
        log_pair_count.push_back(countpairs());
    }
}

int starid::Startriangleside::countpairs() {
    int result = 0;
    for (auto it1 = stars.begin(), end = stars.end(); it1 != end; ++it1) {
        result += it1->second.size();
    }
    return result;
}

starid::Startriangleside2::Startriangleside2(Eigen::Vector3d sv1_, Eigen::Vector3d sv2_, starid::Starpairs &pairs) {
    angtol = .003; sv1 = sv1_; sv2 = sv2_; ang = std::acos(sv1.transpose() * sv2);
    stars = pairs.pairs_for_angle(ang, angtol);
    log_star_count.push_back(stars.size());
    log_pair_count.push_back(countpairs());
}



starid::Startriangleside2::Startriangleside2() {}

int starid::Startriangleside2::countpairs() {
    int result = 0;
    for (auto it1 = stars.begin(), end = stars.end(); it1 != end; ++it1) {
        result += it1->second.size();
    }
    return result;
}

starid::Startriangleside2 &starid::Startriangleside2::operator=(starid::Startriangleside2 other) {
    stars = other.stars;
    return *this;
}
