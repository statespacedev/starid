#include "sky.h"
#include "skymap.h"
#include <random>
#include <fstream>

std::random_device r;
std::default_random_engine e1(r());
std::uniform_real_distribution<double> unitscatter(0, 1);

starid::Sky::Sky() {
}

void starid::Sky::start(std::string pathskymap_) {
    pathskymap = pathskymap_;
    t = 0.0;
    Skymap skymap(pathskymap);
    int starndx = 0;
    for (auto rec: skymap.records) {
        Star star;
        star.starndx = starndx;
        star.skymap_number = rec.skymap_number;
        star.mv = rec.mv1;
        star.ra_degrees = 15.0 * (rec.rah + rec.ram / 60.0 + rec.ras / 3600.0);
        star.dec_degrees = rec.decsign * (rec.decd + rec.decm / 60.0 + rec.decs / 3600.0);
        double ra = star.ra_degrees * starid::pi / 180.0;
        double dec = star.dec_degrees * starid::pi / 180.0;
        star.x = std::cos(ra) * std::cos(dec);
        star.y = std::sin(ra) * std::cos(dec);
        star.z = std::sin(dec);
        xndxer.add_pair(star.x, starndx);
        yndxer.add_pair(star.y, starndx);
        zndxer.add_pair(star.z, starndx);
        stars.push_back(star);
        catalog_lines.push_back(rec.fileline);
        ++starndx;
    }
    xndxer.sort();
    yndxer.sort();
    zndxer.sort();
    return;
}

std::map<std::string, Eigen::MatrixXd> starid::Sky::image_generator(int starndx) {
    using namespace Eigen;
    MatrixXd pixels = MatrixXd::Zero(28, 28);
    MatrixXd info = MatrixXd::Zero(100, 6);
    MatrixXd targets = MatrixXd::Zero(50, 1);
    Vector3d pointing;
    auto star = stars[starndx];
    pointing << star.x, star.y, star.z;
    std::vector<int> starndxs = stars_near_point(pointing(0), pointing(1), pointing(2));
    MatrixXd pvecs = MatrixXd::Zero(100, 3);
    MatrixXd ndxs = MatrixXd::Zero(100, 4);
    int pvecsndx = 0;
    for (auto ndx: starndxs) {
        pvecs.row(pvecsndx) << stars[ndx].x, stars[ndx].y, stars[ndx].z;
        ndxs.row(pvecsndx)
                << stars[ndx].starndx, stars[ndx].skymap_number, stars[ndx].ra_degrees, stars[ndx].dec_degrees;
        ++pvecsndx;
    }
    pvecs.conservativeResize(pvecsndx, 3);
    Matrix3d attitude = rotation_matrix(pointing);
    pvecs = (attitude.transpose() * pvecs.transpose()).transpose();
    double yaw = unitscatter(e1) * 2 * starid::pi;
    int imgindx = 0;
    for (int ndx = 0; ndx < pvecsndx; ++ndx) {
        double x = std::cos(yaw) * pvecs(ndx, 0) - std::sin(yaw) * pvecs(ndx, 1);
        double y = std::sin(yaw) * pvecs(ndx, 0) + std::cos(yaw) * pvecs(ndx, 1);
        double axi = x + starid::image_radius_unit_vector_plane;
        double axj = -y + starid::image_radius_unit_vector_plane;
        int axindx = std::floor(axi / starid::image_pixel_unit_vector_plane);
        int axjndx = std::floor(axj / starid::image_pixel_unit_vector_plane);
        if (ndxs(ndx, 0) == starndx) continue;
        if (axjndx < 0 || axjndx > 27) continue;
        if (axindx < 0 || axindx > 27) continue;
        pixels(axjndx, axindx) = 1.0;
        info(imgindx, 0) = axjndx;
        info(imgindx, 1) = axindx;
        info(imgindx, 2) = ndxs(ndx, 0); // starndx
        info(imgindx, 3) = ndxs(ndx, 1); // skymap_number
        info(imgindx, 4) = ndxs(ndx, 2); // ra
        info(imgindx, 5) = ndxs(ndx, 3); // dec
        ++imgindx;
    }
    targets(0, 0) = starndx;
    std::map<std::string, Eigen::MatrixXd> result;
    result["pixels"] = pixels;
    result["info"] = info;
    result["stars"] = targets;
    return result;
}

std::vector<int> starid::Sky::stars_near_point(double x, double y, double z) {
    double max_ang = 1.4 * starid::image_radius_radians;
    std::vector<int> xring = stars_in_ring(x, max_ang, xndxer);
    std::vector<int> yring = stars_in_ring(y, max_ang, yndxer);
    std::vector<int> zring = stars_in_ring(z, max_ang, zndxer);
    std::vector<int> xy;
    std::set_intersection(xring.begin(), xring.end(), yring.begin(), yring.end(), std::back_inserter(xy));
    std::vector<int> xyz;
    std::set_intersection(xy.begin(), xy.end(), zring.begin(), zring.end(), std::back_inserter(xyz));
    std::vector<int> ndxs;
    for (size_t i = 0; i < xyz.size(); ++i) {
        ndxs.push_back(xyz[i]);
    }
    return ndxs;
}

std::vector<int> starid::Sky::stars_in_ring(double p, double radius, starid::FloatsIndexer &table) {
    double pmin, pmax;
    if (p >= cos(radius)) {
        pmin = p * cos(radius) - sqrt(1 - (p * p)) * sin(radius);
        pmax = 1.0;
    } else if (p <= -cos(radius)) {
        pmin = -1.0;
        pmax = p * cos(radius) + sqrt(1 - (p * p)) * sin(radius);
    } else {
        pmin = p * cos(radius) - sqrt(1 - (p * p)) * sin(radius);
        pmax = p * cos(radius) + sqrt(1 - (p * p)) * sin(radius);
    }
    return table.findndxs(pmin, pmax);
}


