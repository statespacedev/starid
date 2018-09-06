#include "startriangles.h"

starid::startriangles::startriangles(starid::starpairs &starpairs) : starpairs(starpairs) {
    double epsilon = 0.0;
    tolerance = (2.0 * std::sqrt(500.0 * 500.0 + 500.00 * 500.0) + epsilon) * starid::arcseconds_to_radians;
}

int starid::startriangles::identify(Eigen::MatrixXd &pixels, int teststar) {
    starvecs = starid::pixels_to_starvecs(pixels);
    std::vector<startriangleside> abs;
    for (ndxb = 1; ndxb < starvecs.rows(); ++ndxb) {
        uveca = starvecs.row(0);
        uvecb = starvecs.row(ndxb);
        startriangleside ab(std::acos(uveca.transpose() * uvecb), tolerance, starpairs, teststar);
        int prev_stars = 0;
        int repeatcnt = 0;
        bool converged = false;
        for (ndxc = 1; ndxc < starvecs.rows(); ++ndxc) {
            if (converged || !get_angs_c()) continue;
            startriangle abca(angs_c[0], angs_c[1], angs_c[2], tolerance, starpairs, teststar, starvecs.row(ndxc).transpose());
            abca.side1.stars = ab.stars;
            abca.close_loops_abca();
            ab.append_iterations(abca.side1);
            std::vector<startriangle> triangles;
            triangles.push_back(abca);
            for (ndxd = 1; ndxd < starvecs.rows(); ++ndxd) {
                if (converged || !get_angs_d()) continue;
                startriangle abda(angs_d[0], angs_d[4], angs_d[3], tolerance, starpairs, teststar, starvecs.row(ndxd).transpose());
                abda.side1.stars = ab.stars;
                abda.close_loops_abda(triangles);
                ab.append_iterations(abda.side1);
                triangles.push_back(abda);
                if (prev_stars == ab.stars.size()) ++repeatcnt; else repeatcnt = 0;
                if (repeatcnt > 3) converged = true;
                prev_stars = ab.stars.size();
                if (ab.stars.size() == 1) break;
            }
            if (ab.stars.size() == 1) break;
        }
        if (ab.stars.size() == 1) {
            auto starsit = ab.stars.begin();
            return starsit->first;
        }
        abs.push_back(ab);
    }
    return -1;
}

bool starid::startriangles::get_angs_d() {
    if (ndxd == ndxb || ndxd == ndxc) return false;
    bool angsok = true;
    angs_d = angs_c;
    uvecd = starvecs.row(ndxd);
    angs_d.push_back(std::acos(uvecd.transpose() * uveca));
    angs_d.push_back(std::acos(uvecd.transpose() * uvecb));
    angs_d.push_back(std::acos(uvecd.transpose() * uvecc));
    if (angs_d[3] < min_ang) angsok = false; // da
    if (angs_d[4] < min_ang) angsok = false; // db
    if (angs_d[5] < min_ang) angsok = false; // dc
    if (std::abs(angs_d[4] - angs_d[3]) < min_ang) angsok = false; // db-da
    //if (std::abs(angs_d[4]-angs_d[0]) < min_ang) angsok = false; // db-ab
    //if (std::abs(angs_d[4]-angs_d[5]) < min_ang) angsok = false; // db-dc
    return angsok;
}

bool starid::startriangles::get_angs_c() {
    if (ndxc == ndxb) return false;
    bool angsok = true;
    angs_c.clear();
    uvecc = starvecs.row(ndxc);
    angs_c.push_back(std::acos(uveca.transpose() * uvecb));
    angs_c.push_back(std::acos(uvecb.transpose() * uvecc));
    angs_c.push_back(std::acos(uvecc.transpose() * uveca));
    min_ang = 3000.0 * starid::arcseconds_to_radians;
    if (angs_c[0] < min_ang) angsok = false; // ab
    if (angs_c[1] < min_ang) angsok = false; // bc
    if (angs_c[2] < min_ang) angsok = false; // ca
    if (std::abs(angs_c[0] - angs_c[1]) < min_ang) angsok = false; // ab-bc
    if (std::abs(angs_c[0] - angs_c[2]) < min_ang) angsok = false; // ab-ca
    if (std::abs(angs_c[1] - angs_c[2]) < min_ang) angsok = false; // bc-ca
    return angsok;
}

starid::startriangle::startriangle(double ang1, double ang2, double ang3, double tolerance, starid::starpairs &pairs,
                           int teststar, Eigen::Vector3d vecin)
        : side1(ang1, tolerance, pairs, teststar), side2(ang2, tolerance, pairs, teststar), side3(ang3, tolerance, pairs, teststar),
          teststar(teststar), tolerance(tolerance), pairs(pairs), vecstar3(vecin) {
    vecstar3 << vecin(0), vecin(1), vecin(2);
}

void starid::startriangle::close_loops_abda(std::vector<startriangle> &triangles) {
    int maxtriangles = triangles.size();
    for (int trianglendx = 0; trianglendx < maxtriangles; ++trianglendx) {
        double cdang = std::acos(vecstar3.transpose() * triangles[trianglendx].vecstar3);
        startriangleside cd(cdang, tolerance, pairs, teststar);
        loops_cnt = 0;
        for (auto it11 = side1.stars.begin(), end = side1.stars.end(); it11 != end; ++it11) {
            auto &pairs1 = it11->second;
            int star1side1 = it11->first;                     // star1 side1
            auto star1side3 = side3.stars.find(star1side1);   // star1 side3
            if (star1side3 == side3.stars.end()) continue;
            auto &pairs3 = star1side3->second;
            for (auto pairs1it = pairs1.begin(), end = pairs1.end(); pairs1it != end; ++pairs1it) {
                int star2side1 = pairs1it->first;               // star2 side1
                auto star2side2 = side2.stars.find(star2side1); // star2 side2
                if (star2side2 == side2.stars.end()) continue;
                auto &pairs2 = star2side2->second;
                for (auto pairs2it = pairs2.begin(), end = pairs2.end(); pairs2it != end; ++pairs2it) {
                    int star3side2 = pairs2it->first;             // star3 side2
                    auto star3side3 = pairs3.find(star3side2);    // star3 side3
                    if (star3side3 == pairs3.end()) continue;
                    bool dok = false; // is this d star possible
                    auto cdd = cd.stars.find(star3side2);
                    auto aca = triangles[trianglendx].side3.stars.find(star1side1);
                    if (cdd != cd.stars.end() && aca != triangles[trianglendx].side3.stars.end()) {
                        auto &pairscdc = cdd->second;
                        auto &pairsacc = aca->second;
                        for (auto pairscdcit = pairscdc.begin(), end = pairscdc.end();
                             pairscdcit != end; ++pairscdcit) {
                            auto pairsaccit = pairsacc.find(pairscdcit->first);
                            if (pairsaccit != pairsacc.end()) {
                                dok = true;
                                break;
                            }
                        }
                    }
                    if (!dok) continue;
                    pairs1it->second = 1;
                    pairs2it->second = 1;
                    star3side3->second = 1;
                    ++loops_cnt;
                }
            }
        }
        side1.trim_pairs();
        side2.trim_pairs();
        side3.trim_pairs();
        if (side1.stars.size() == 1) break;
    }
}

void starid::startriangle::close_loops_abca() {
    loops_cnt = 0;
    for (auto it11 = side1.stars.begin(), end = side1.stars.end(); it11 != end; ++it11) {
        auto &pairs1 = it11->second;
        int star1side1 = it11->first;                     // star1 side1
        auto star1side3 = side3.stars.find(star1side1);   // star1 side3
        if (star1side3 == side3.stars.end()) continue;
        auto &pairs3 = star1side3->second;
        for (auto pairs1it = pairs1.begin(), end = pairs1.end(); pairs1it != end; ++pairs1it) {
            int star2side1 = pairs1it->first;               // star2 side1
            auto star2side2 = side2.stars.find(star2side1); // star2 side2
            if (star2side2 == side2.stars.end()) continue;
            auto &pairs2 = star2side2->second;
            for (auto pairs2it = pairs2.begin(), end = pairs2.end(); pairs2it != end; ++pairs2it) {
                int star3side2 = pairs2it->first;             // star3 side2
                auto star3side3 = pairs3.find(star3side2);    // star3 side3
                if (star3side3 == pairs3.end()) continue;
                pairs1it->second = 1;
                pairs2it->second = 1;
                star3side3->second = 1;
                ++loops_cnt;
            }
        }
    }
    side1.trim_pairs();
    side2.trim_pairs();
    side3.trim_pairs();
}

starid::startriangleside::startriangleside(double ang, double tolerance, starid::starpairs &pairs, int starndx)
        : teststar(starndx) {
    stars = pairs.pairsndxr(ang, tolerance);
}

void starid::startriangleside::append_iterations(startriangleside &side) {
    stars = side.stars;
    for (auto tmp : side.log_pair_count) log_pair_count.push_back(tmp);
    for (auto tmp : side.log_star_count) log_star_count.push_back(tmp);
    for (auto tmp : side.log_teststar) log_teststar.push_back(tmp);
    has_teststar = side.has_teststar;
}

void ::starid::startriangleside::trim_pairs() {
    for (auto star1 = stars.begin(), end = stars.end(); star1 != end; ++star1) {
        auto &pairs = star1->second;
        for (auto star2 = pairs.begin(); star2 != pairs.end();) {
            if (star2->second == 0) {
                star2 = pairs.erase(star2);
            } else {
                star2->second = 0;
                ++star2;
            }
        }
    }
    for (auto star1 = stars.begin(); star1 != stars.end();) {
        auto &pairs = star1->second;
        if (pairs.empty())
            star1 = stars.erase(star1);
        else
            ++star1;
    }
    has_teststar = check_teststar(teststar);
    log_star_count.push_back(stars.size());
    log_pair_count.push_back(pair_count());
    log_teststar.push_back(has_teststar);
}

int starid::startriangleside::pair_count() {
    int result = 0;
    for (auto it1 = stars.begin(), end = stars.end(); it1 != end; ++it1) {
        result += it1->second.size();
    }
    return result;
}

std::map<int, int> starid::startriangleside::summary() {
    std::map<int, int> result;
    for (auto it = stars.begin(), end = stars.end(); it != end; ++it) {
        auto &inner = it->second;
        result.emplace(it->first, inner.size());
    }
    return result;
}

bool starid::startriangleside::check_teststar(int starndx) {
    auto it = stars.find(starndx);
    if (it == stars.end()) return false;
    return true;
}

void starid::starpairs::start(starid::sky &sky) {
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
                double angle = std::acos((sky.stars[starndx1].x * sky.stars[starndx2].x) +
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

std::unordered_map<int, std::unordered_map<int, int>> starid::starpairs::pairsndxr(double angle, double tol_radius) {
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
};

std::string starid::starpairs::pairlabeler(int catndx1, int catndx2) {
    if (catndx1 > catndx2) {
        int tmp = catndx1;
        catndx1 = catndx2;
        catndx2 = tmp;
    }
    std::string label = std::to_string(catndx1) + std::to_string(catndx2);
    return label;
}