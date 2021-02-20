#include "sky.h"
#include "skymap.h"
#include <random>
#include <fstream>

std::random_device r;
std::default_random_engine e1(r());
std::uniform_real_distribution<double> unitscatter(0, 1);

/*
 * class Sky:
 *    '''model the sky, based on the skymap object. the key input parameter is the star brightness threshold - with visual magnitude 6.5 the sky is about nine thousand stars, and that number grows exponentially as dimmer stars are included.'''
 * */
starid::Sky::Sky() {
}

/*
 *    def start(self, pathin):
 *       '''initialize the sky. first generates a skymap object and then picks out the information needed here, with some enrichment - in particular with three-dimensional vectors in the celestial reference frame.'''
 * */
void starid::Sky::start(std::string pathin) {
    pathcat = pathin;
    t = 0.0;
    star star;
    Skymap skymap(pathcat);
    int starndx = 0;
    for (auto rec : skymap.records) {
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
}

/*
 *    def image_generator(self, starndx):
 *       '''creates a standard image for the target star, ready for feeding into a star identifier. the format is 28 x 28 pixels - lo-fi, the way we like it. makes thing tougher on us. and also by no coincidence matching the classic mnist character recognition data set. the story behind that is a long one, discussed elsewhere in the project.'''
 * */
std::map<std::string, Eigen::MatrixXd> starid::Sky::image_generator(int starndx) {
    using namespace Eigen;
    MatrixXd pixels = MatrixXd::Zero(28, 28);
    MatrixXd info = MatrixXd::Zero(100, 6);
    MatrixXd targets = MatrixXd::Zero(50, 1);
    Vector3d pointing;
    pointing << stars[starndx].x, stars[starndx].y, stars[starndx].z;
    std::vector<int> starndxs = stars_near_point(pointing(0), pointing(1), pointing(2));
    MatrixXd pvecs = MatrixXd::Zero(100, 3);
    MatrixXd ndxs = MatrixXd::Zero(100, 4);
    int pvecsndx = 0;
    for (auto ndx : starndxs) {
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

/*
 *    def stars_near_point(self, x, y, z):
 *       '''given a three-dimensional pointing vector in the celestial reference frame, return the identifiers for nearby stars. this is fundamental - we have to be able to call up the stars near a target on the sky. it's a rich problem we'll be discussing throughout the project documentation. here we break the three-dimensional search space down into three one-dimensional search spaces, and create a map or hash-index into each of those. in a sense - it's a three-dimensional hash map into the sky.'''
 * */
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
    for (uint i = 0; i < xyz.size(); ++i) {
        ndxs.push_back(xyz[i]);
    }
    return ndxs;
}

/*
 *    def stars_in_ring(self, p, radius, table):
 *       '''when we break the skies three-dimensional search space down into three one-dimensional search spaces, the one-dimensional spaces represent rings on the sky. we have three rings, and the stars we're interested in are in their intersection. this intersection of three rings is in some sense a three-dimensional hash map into the sky.'''
 * */
std::vector<int> starid::Sky::stars_in_ring(double p, double radius, starid::range_of_floats_indexer &table) {
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


