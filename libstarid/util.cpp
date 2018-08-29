#include "util.h"

double starid::image_radius_radians = 0.0698131700797732; // four degrees in radians
double starid::star_pair_angle_limit = 0.197461463918150; // 2*sqrt(2)*image_radius_radians
double starid::image_radius_unit_vector_plane = 0.0697564737441253; // sin(4*pi/180)
double starid::image_pixel_unit_vector_plane = 0.00498260526743752; // sin(4*pi/180)/14
double starid::star_brightness_limit = 6.5; // star visual magnitude
double starid::pi = 3.14159265358979323846;
double starid::arcseconds_to_radians = starid::pi / 648000.0;

void starid::range_of_floats_indexer::add_pair(double newFloat, int newInt) {
    std::pair<double, int> pair{newFloat, newInt};
    float_int_table.push_back(pair);
}

void starid::range_of_floats_indexer::sort() {
    std::sort(float_int_table.begin(), float_int_table.end());
}

std::vector<int> starid::range_of_floats_indexer::find_ints(double lowerFloat, double upperFloat) {
    std::vector<int> intsFromTable;
    auto itlow = std::lower_bound(float_int_table.begin(), float_int_table.end(),
                                  std::make_pair(lowerFloat, 0));
    auto ithi = std::upper_bound(float_int_table.begin(), float_int_table.end(),
                                 std::make_pair(upperFloat, 0));
    if (itlow == ithi) {
        return intsFromTable;
    }
    for (auto it = itlow; it <= ithi; ++it) {
        auto tableRow = *it;
        intsFromTable.push_back(tableRow.second);
    }
    std::sort(intsFromTable.begin(), intsFromTable.end());
    return intsFromTable;
}
