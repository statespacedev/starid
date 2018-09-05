#ifndef STARIDENTIFIER_H
#define STARIDENTIFIER_H

#include <Eigen/Core>
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/unordered_map.hpp>
#include <unordered_map>
#include <map>
#include "sky.h"
#include "util.h"

namespace starid {

    class starpairs {
    public:
        void start(starid::sky &sky);
        std::unordered_map<int, std::unordered_map<int, int>> pairs_map(double angle, double tol_radius);
    private:
        starid::range_of_floats_indexer angletable; // angle, starpairs ndx
        std::vector<std::tuple<double, int, int>> starpairs; // angle, catndx1, catndx2
        std::unordered_map<std::string, int> starpairs_map; // starpairkey, starpairsndx
        std::string pairs_key(int catndx1, int catndx2); // hash key
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive &ar) {
            ar(starpairs, starpairs_map, angletable);
        }
    };

    class startriangleside {
    public:
        std::unordered_map<int, std::unordered_map<int, int>> stars;
        void trim_pairs();
        void append_iterations(startriangleside &side);
        std::vector<int> log_star_count;
        std::vector<int> log_pair_count;
        std::vector<bool> log_teststar;
        int teststar;
        bool has_teststar;
        startriangleside(double ang, double tolerance, starid::starpairs &pairs, int teststar);
        startriangleside(int teststar);
        std::map<int, int> summary();
        bool check_teststar(int teststar);
        int pair_count();
    private:
    };

    class startriangle {
    public:
        void close_loops_abca();
        void close_loops_abda(std::vector<startriangle> &triangles);
        startriangle(double ang1, double ang2, double ang3, double tolerance, starid::starpairs &pairs, int teststar, Eigen::Vector3d vecstar3);
        starid::startriangleside side1;
        starid::startriangleside side2;
        starid::startriangleside side3;
        int loops_cnt;
        int teststar;
        double tolerance;
        starid::starpairs &pairs;
        Eigen::Vector3d vecstar3;
    private:
    };

    class startriangles {
    public:
//        triangles(starid::image_matrix &imgmat, starid::pairs &pairs);
        int id(int teststar = -1);
    private:
        bool get_angs_c();
        bool get_angs_d();
        starid::starpairs pairs;
        Eigen::MatrixXd pvecs;
        double tolerance;
        double min_ang;
        std::vector<double> angs_c;
        std::vector<double> angs_d;
        Eigen::Vector3d uveca;
        Eigen::Vector3d uvecb;
        Eigen::Vector3d uvecc;
        Eigen::Vector3d uvecd;
        int ndxb, ndxc, ndxd;
        int teststar;
    };
}

#endif
