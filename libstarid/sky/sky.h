#ifndef SKY_H
#define SKY_H

#include "include/Eigen/Core"
#include <map>
#include "geometry.h"

namespace starid {

    struct Star {
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

    class Sky {
    public:
        Sky();

        void generate(std::string pathskymap_);

        std::vector<Star> stars;
        std::vector<std::string> catalog_lines;

        std::vector<int> stars_near_point(double x, double y, double z);

        std::map<std::string, Eigen::MatrixXd> image_generator(int starndx);

    private:
        double t;
        std::string pathskymap;
        starid::FloatsIndexer xndxer;
        starid::FloatsIndexer yndxer;
        starid::FloatsIndexer zndxer;

        std::vector<int> stars_in_ring(double p, double radius, starid::FloatsIndexer &table);

        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &ar) {
            ar(stars, xndxer, yndxer, zndxer);
        }
    };
}

#endif

