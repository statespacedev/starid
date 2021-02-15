#ifndef SKY_H
#define SKY_H
#include <Eigen/Core>
#include <map>
#include "util.h"

namespace starid {

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
    
    class Skymap {
    public:
        explicit Skymap(std::string pathcat);
        std::vector<skymaprec> records;
    };

    class Sky {
    public:
        Sky();
        void start(std::string pathin);
        std::vector<star> stars;
        std::vector<std::string> catalog_lines;
        std::vector<int> stars_near_point(double x, double y, double z);
        std::map<std::string, Eigen::MatrixXd> image_generator(int starndx);
        std::map<std::string, Eigen::MatrixXd> angle_generator(int starndx);
    private:
        double t;
        std::string pathcat;
        starid::range_of_floats_indexer xndxer;
        starid::range_of_floats_indexer yndxer;
        starid::range_of_floats_indexer zndxer;
        std::vector<int> stars_in_ring(double p, double radius, starid::range_of_floats_indexer &table);
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive &ar) {
            ar(stars, xndxer, yndxer, zndxer);
        }
    };
}

#endif

