#include "util.h"

double starid::pairangle(Eigen::MatrixXd &starvecs, int ndxa, int ndxb) {
    Eigen::Vector3d uvec1 = starvecs.row(ndxa);
    Eigen::Vector3d uvec2 = starvecs.row(ndxb);
    return std::acos(uvec1.transpose() * uvec2);
}

void starid::range_of_floats_indexer::add_pair(double newfloat, int newndx) {
    std::pair<double, int> pair{newfloat, newndx};
    float_ndx.push_back(pair); }

void starid::range_of_floats_indexer::sort() { std::sort(float_ndx.begin(), float_ndx.end()); }

std::vector<int> starid::range_of_floats_indexer::findndxs(double lofloat, double hifloat) {
    std::vector<int> intsFromTable;
    auto itlow = std::lower_bound(float_ndx.begin(), float_ndx.end(),std::make_pair(lofloat, 0));
    auto ithi = std::upper_bound(float_ndx.begin(), float_ndx.end(),std::make_pair(hifloat, 0));
    if (itlow == ithi) return intsFromTable;
    for (auto it = itlow; it <= ithi; ++it) { auto tableRow = *it; intsFromTable.push_back(tableRow.second); }
    std::sort(intsFromTable.begin(), intsFromTable.end());
    return intsFromTable;
}

Eigen::MatrixXd starid::pixels_to_starvecs(Eigen::MatrixXd &imgs) {
    Eigen::MatrixXd pvecs = Eigen::MatrixXd::Zero(100, 3);
    pvecs.row(0) << 0.0, 0.0, 1.0;
    int pvecsndx = 1;
    for (int axjndx = 0; axjndx < 28; ++axjndx) {
        for (int axindx = 0; axindx < 28; ++axindx) {
            if (imgs(axjndx, axindx) > 0) { // there's a star inside axjndx, axindx
                double x = starid::image_pixel_unit_vector_plane * (-13.5 + (double) axindx);
                double y = starid::image_pixel_unit_vector_plane * (+13.5 - (double) axjndx);
                pvecs.row(pvecsndx) << x, y, std::sqrt(1 - x * x - y * y);
                ++pvecsndx;
            }
        }
    }
    pvecs.conservativeResize(pvecsndx, 3);
    return pvecs;
}

Eigen::Matrix3d starid::rotation_matrix(Eigen::Vector3d &bodyz) {
    using namespace Eigen;
    Matrix3d rm = Matrix3d::Identity(3, 3);
    Vector3d icrfz(0.0, 0.0, 1.0);
    Vector3d bodyx = crossprod(bodyz, icrfz);
    Vector3d bodyy = crossprod(bodyz, bodyx);
    rm.col(0) = bodyx.normalized();
    rm.col(1) = bodyy.normalized();
    rm.col(2) = bodyz.normalized();
    return rm;
}

Eigen::Vector3d starid::crossprod(Eigen::Vector3d &u, Eigen::Vector3d &v) {
    Eigen::Vector3d result;
    result(0) = u(1) * v(2) - u(2) * v(1);
    result(1) = u(2) * v(0) - u(0) * v(2);
    result(2) = u(0) * v(1) - u(1) * v(0);
    return result;
}

