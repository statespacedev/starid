#ifndef PROJECT_UTIL_H
#define PROJECT_UTIL_H
#include <cereal/access.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <algorithm>
#include <chrono>

namespace starid {
    extern double image_radius_radians;
    extern double image_radius_unit_vector_plane;
    extern double image_pixel_unit_vector_plane;
    extern double star_brightness_limit;
    extern double star_pair_angle_limit;
    extern double pi;
    extern double arcseconds_to_radians;

    class float_int_table {
    public:
        void add_pair(double newFloat, int newInt);
        void sort();
        std::vector<int> find_ints(double lowerFloat, double upperFloat);
    private:
        std::vector<std::pair<double, int>> float_int_table;
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive &ar) {
            ar(float_int_table);
        }
    };
}

#endif //PROJECT_UTIL_H
