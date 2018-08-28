#ifndef SKY_H
#define SKY_H
#include <Eigen/Core>
#include <cereal/access.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <random>
#include "util.h"

namespace starid {
    using images = Eigen::MatrixXd;
    using ang_seq_vec = Eigen::Matrix<double, 36, 1>;

    class skymap {
    public:
        struct skymaprec {
            double mv1;
            std::string iau_identifier;
            std::string star_name;
            std::string variablestar_name;
            int skymap_number;
            int hd_number;
            int sao_number;
            int dm_number;
            int hr_number;
            int wds_number;
            int ppm_number;
            bool blended_position;
            double rah;
            double ram;
            double ras;
            double decd;
            double decm;
            double decs;
            double pmra_arcsec_per_year;
            double pmdec_arcsec_per_year;
            double decsign;
            double pmdecsign;
            std::string fileline;
        };
        skymap(std::string fcat);
        std::vector<skymaprec> star_records;
        int dim_stars;
    };

    class sky {
    public:
        struct star {
            int starndx;
            int skymap_number;
            double mv;
            double ra_degrees;
            double dec_degrees;
            double x;
            double y;
            double z;
            template<class Archive>
            void serialize(Archive &ar) {
                ar(starndx, skymap_number, mv, ra_degrees, dec_degrees, x, y, z);
            }
        };
        std::vector<starid::sky::star> stars;
        std::vector<std::string> catalog_lines;
        void init(std::string fcatalog);
        std::vector<int> stars_near_point(double x, double y, double z);
        static images image_generator(int starndx, starid::sky &sky);
        static ang_seq_vec ang_seq_generator(int starndx, starid::sky &sky);
        static Eigen::MatrixXd get_pvecs_from_images(images &imgs);
    private:
        double t;
        std::string fcatalog;
        starid::float_int_table xtable;
        starid::float_int_table ytable;
        starid::float_int_table ztable;
        std::vector<int> stars_in_ring(double p, double radius, starid::float_int_table &table);
        static Eigen::Matrix3d rotation_matrix(Eigen::Vector3d &pointing);
        static Eigen::Vector3d crossprod(Eigen::Vector3d &u, Eigen::Vector3d &v);
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive &ar) {
            ar(stars, xtable, ytable, ztable);
        }
    };
}

#endif

