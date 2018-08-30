#ifndef SKY_H
#define SKY_H
#include <Eigen/Core>
#include <map>
#include "util.h"

namespace starid {
    class skymap {
    public:
        struct record {
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
        skymap(std::string pathcat);
        std::vector<record> records;
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
        void init(std::string pathin);
        std::vector<int> stars_near_point(double x, double y, double z);
        static std::map<std::string, Eigen::MatrixXd> image_generator(int starndx, starid::sky &sky);
        static Eigen::Matrix<double, 36, 1> ang_seq_generator(int starndx, starid::sky &sky);
        static Eigen::MatrixXd get_pvecs_from_images(Eigen::MatrixXd &imgs);
    private:
        double t;
        std::string pathcat;
        starid::range_of_floats_indexer xndxer;
        starid::range_of_floats_indexer yndxer;
        starid::range_of_floats_indexer zndxer;
        std::vector<int> stars_in_ring(double p, double radius, starid::range_of_floats_indexer &table);
        static Eigen::Matrix3d rotation_matrix(Eigen::Vector3d &pointing);
        static Eigen::Vector3d crossprod(Eigen::Vector3d &u, Eigen::Vector3d &v);
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive &ar) {
            ar(stars, xndxer, yndxer, zndxer);
        }
    };
}

#endif

