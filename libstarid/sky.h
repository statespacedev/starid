#ifndef SKY_H
#define SKY_H
#include <Eigen/Core>
#include <map>
#include "util.h"

namespace starid {

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

class Sky {
public:
    Sky();
    void start(std::string pathin);
    std::vector<star> stars;
    std::vector<std::string> catalog_lines;
    std::vector<int> stars_near_point(double x, double y, double z);
    std::map<std::string, Eigen::MatrixXd> image_generator(int starndx);
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

