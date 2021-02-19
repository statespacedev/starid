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
#include "starpairs.h"

namespace starid {

class Startriangles {
    public:
        explicit Startriangles(Starpairs &pairs);
        int identify(Eigen::MatrixXd &pixels, int teststar = -1);
    private:
        bool get_angs_c();
        bool get_angs_d();
        Starpairs starpairs;
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

    class Startriangleside {
    public:
        std::unordered_map<int, std::unordered_map<int, int>> stars;
        void trim_pairs();
        void append_iterations(Startriangleside &side);
        std::vector<int> log_star_count;
        std::vector<int> log_pair_count;
        std::vector<bool> log_teststar;
        int teststar;
        bool has_teststar;
        Startriangleside(double ang, double tolerance, Starpairs &pairs, int teststar);
        std::map<int, int> summary();
        bool check_teststar(int teststar);
        int pair_count();
    private:
    };

    class Startriangle {
    public:
        void close_loops_abca();
        void close_loops_abda(std::vector<Startriangle> &triangles);
        Startriangle(double ang1, double ang2, double ang3, double tolerance,
                     Starpairs &pairs, int teststar, Eigen::Vector3d vecstar3);
        starid::Startriangleside side1;
        starid::Startriangleside side2;
        starid::Startriangleside side3;
        int loops_cnt;
        int teststar;
        double tolerance;
        Starpairs &pairs;
        Eigen::Vector3d vecstar3;
    private:
    };
}

#endif
