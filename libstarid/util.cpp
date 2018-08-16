#include "util.h"

void starid::float_int_table::add_pair(double newFloat, int newInt) {
    std::pair<double, int> pair{newFloat, newInt};
    float_int_table.push_back(pair);
}

void starid::float_int_table::sort() {
    std::sort(float_int_table.begin(), float_int_table.end());
}

std::vector<int> starid::float_int_table::find_ints(double lowerFloat, double upperFloat) {
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

starid::stopwatch::stopwatch() {
    t1 = std::chrono::steady_clock::now();
}

int starid::stopwatch::end() {
    t2 = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
}

void starid::stopwatch::reset() {
    t1 = std::chrono::steady_clock::now();
}