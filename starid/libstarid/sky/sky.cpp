#include "sky.h"
#include "skymap.h"
#include <random>
#include <fstream>
using namespace starid;

std::random_device r;
std::default_random_engine e1(r());
std::uniform_real_distribution<double> unitscatter(0, 1);

Sky::Sky() {}

void Sky::generate(std::string pathskymap_) {
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
        double ra = star.ra_degrees * pi / 180.0;
        double dec = star.dec_degrees * pi / 180.0;
        star.x = std::cos(ra) * std::cos(dec);
        star.y = std::sin(ra) * std::cos(dec);
        star.z = std::sin(dec);
        xndxer.add_pair(star.x, starndx);
        yndxer.add_pair(star.y, starndx);
        zndxer.add_pair(star.z, starndx);
        stars.push_back(star);
        catalog_lines.push_back(rec.fileline);
        ++starndx;}
    xndxer.sort();
    yndxer.sort();
    zndxer.sort();
    return;}

std::map<std::string, MatrixXd> Sky::image_generator(int starndx) {
    MatrixXd pixels = MatrixXd::Zero(28, 28);
    MatrixXd info = MatrixXd::Zero(100, 6);
    Vector3d pointing;
    auto target = stars[starndx];
    pointing << target.x, target.y, target.z;
    std::vector<int> starndxs = stars_near_point(pointing(0), pointing(1), pointing(2));
    MatrixXd pvecs = MatrixXd::Zero(100, 3);
    MatrixXd ndxs = MatrixXd::Zero(100, 4);
    int pvecsndx = 0;
    for (auto ndx: starndxs) {
        pvecs.row(pvecsndx) << stars[ndx].x, stars[ndx].y, stars[ndx].z;
        ndxs.row(pvecsndx) << stars[ndx].starndx, stars[ndx].skymap_number, stars[ndx].ra_degrees, stars[ndx].dec_degrees;
        ++pvecsndx;}
    pvecs.conservativeResize(pvecsndx, 3);
    Matrix3d attitude = rotation_matrix(pointing);
    pvecs = (attitude.transpose() * pvecs.transpose()).transpose();
    double yaw = 0; // todo unitscatter(e1) * 2 * pi;
    int imgindx = 0;
    for (int ndx = 0; ndx < pvecsndx; ++ndx) {
        if (ndxs(ndx, 0) == starndx) continue; // target star is implicit in the results
        double x = std::cos(yaw) * pvecs(ndx, 0) - std::sin(yaw) * pvecs(ndx, 1);
        double y = std::sin(yaw) * pvecs(ndx, 0) + std::cos(yaw) * pvecs(ndx, 1);
        double axi = x + image_radius_unit_vector_plane;
        double axj = -y + image_radius_unit_vector_plane;
        int axindx = std::floor(axi / image_pixel_unit_vector_plane);
        int axjndx = std::floor(axj / image_pixel_unit_vector_plane);
        if (axjndx < 0 || axjndx > 27) continue;
        if (axindx < 0 || axindx > 27) continue;
        pixels(axjndx, axindx) = 1.0;
        info(imgindx, 0) = axjndx;
        info(imgindx, 1) = axindx;
        info(imgindx, 2) = ndxs(ndx, 0); // starndx
        info(imgindx, 3) = ndxs(ndx, 1); // skymap_number
        info(imgindx, 4) = ndxs(ndx, 2); // ra
        info(imgindx, 5) = ndxs(ndx, 3); // dec
        ++imgindx;}
    std::map<std::string, MatrixXd> imgdata;
    imgdata["pixels"] = pixels;
    imgdata["info"] = info;
    return imgdata;}

std::vector<int> Sky::stars_near_point(double x, double y, double z) {
    double max_ang = 1.4 * image_radius_radians;
    std::vector<int> xring = stars_in_ring(x, max_ang, xndxer);
    std::vector<int> yring = stars_in_ring(y, max_ang, yndxer);
    std::vector<int> zring = stars_in_ring(z, max_ang, zndxer);
    std::vector<int> xy;
    std::set_intersection(xring.begin(), xring.end(), yring.begin(), yring.end(), std::back_inserter(xy));
    std::vector<int> xyz;
    std::set_intersection(xy.begin(), xy.end(), zring.begin(), zring.end(), std::back_inserter(xyz));
    std::vector<int> ndxs;
    for (size_t i = 0; i < xyz.size(); ++i) ndxs.push_back(xyz[i]);
    return ndxs;}

std::vector<int> Sky::stars_in_ring(double p, double radius, FloatsIndexer &table) {
    double pmin, pmax;
    if (p >= cos(radius)) {
        pmin = p * cos(radius) - sqrt(1 - (p * p)) * sin(radius);
        pmax = 1.0;
    } else if (p <= -cos(radius)) {
        pmin = -1.0;
        pmax = p * cos(radius) + sqrt(1 - (p * p)) * sin(radius);
    } else {
        pmin = p * cos(radius) - sqrt(1 - (p * p)) * sin(radius);
        pmax = p * cos(radius) + sqrt(1 - (p * p)) * sin(radius);}
    return table.findndxs(pmin, pmax);}
