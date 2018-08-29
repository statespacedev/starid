#include "util.h"

void starid::range_of_floats_indexer::add_pair(double newfloat, int newndx) {
    std::pair<double, int> pair{newfloat, newndx};
    float_ndx_table.push_back(pair);
}

void starid::range_of_floats_indexer::sort() {
    std::sort(float_ndx_table.begin(), float_ndx_table.end());
}

std::vector<int> starid::range_of_floats_indexer::findndxs(double lofloat, double hifloat) {
    std::vector<int> intsFromTable;
    auto itlow = std::lower_bound(float_ndx_table.begin(), float_ndx_table.end(),
                                  std::make_pair(lofloat, 0));
    auto ithi = std::upper_bound(float_ndx_table.begin(), float_ndx_table.end(),
                                 std::make_pair(hifloat, 0));
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
