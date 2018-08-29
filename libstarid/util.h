#ifndef PROJECT_UTIL_H
#define PROJECT_UTIL_H
#include <cereal/access.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <algorithm>
#include <chrono>

namespace starid {
    const double image_radius_radians = 0.0698131700797732; // four degrees in radians
    const double star_pair_angle_limit = 0.197461463918150; // 2*sqrt(2)*image_radius_radians
    const double image_radius_unit_vector_plane = 0.0697564737441253; // sin(4*pi/180)
    const double image_pixel_unit_vector_plane = 0.00498260526743752; // sin(4*pi/180)/14
    const double star_brightness_limit = 6.5; // star visual magnitude
    const double pi = 3.14159265358979323846;
    const double arcseconds_to_radians = starid::pi / 648000.0;

    class range_of_floats_indexer {
    public:
        void add_pair(double newfloat, int newndx);
        void sort();
        std::vector<int> findndxs(double lofloat, double hifloat);
    private:
        std::vector<std::pair<double, int>> float_ndx_table;
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive &ar) {
            ar(float_ndx_table);
        }
    };
}

#endif //PROJECT_UTIL_H
