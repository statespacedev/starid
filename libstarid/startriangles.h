#ifndef STARTRIANGLES_H
#define STARTRIANGLES_H

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
        void start(starid::Sky &sky);
        std::unordered_map<int, std::unordered_map<int, int>> pairsndxr(double angle, double tol_radius);
    private:
        starid::range_of_floats_indexer angndxs;
        std::vector<std::tuple<double, int, int>> starpairs;
        std::unordered_map<std::string, int> starpairsndxs;
        std::string pairlabeler(int catndx1, int catndx2);
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive &ar) {
            ar(starpairs, starpairsndxs, angndxs);
        }
    };

    class Startriangles {
    public:
        explicit Startriangles(starid::starpairs &pairs);
        int identify(Eigen::MatrixXd &pixels, int teststar = -1);
    private:
        bool get_angs_c();
        bool get_angs_d();
        starid::starpairs starpairs;
        Eigen::MatrixXd starvecs;
        double tolerance;
        double min_ang;
        std::vector<double> angs_c;
        std::vector<double> angs_d;
        Eigen::Vector3d uveca;
        Eigen::Vector3d uvecb;
        Eigen::Vector3d uvecc;
        Eigen::Vector3d uvecd;
        int ndxb, ndxc, ndxd;
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
        std::map<int, int> summary();
        bool check_teststar(int teststar);
        int pair_count();
    private:
    };

    class startriangle {
    public:
        void close_loops_abca();
        void close_loops_abda(std::vector<startriangle> &triangles);
        startriangle(double ang1, double ang2, double ang3, double tolerance,
                     starid::starpairs &pairs, int teststar, Eigen::Vector3d vecstar3);
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
}

#endif
