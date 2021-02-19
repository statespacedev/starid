#ifndef STARID_SKYMAP_H
#define STARID_SKYMAP_H
#include <Eigen/Core>
#include <map>
#include "util.h"

namespace starid {

struct skymaprec {
    double mv1;
    std::string iau_identifier;
    std::string star_name;
    std::string variablestar_name;
    int skymap_number;
    int hd_number;
    int sao_number;
    int dm_number;
    int hr_number;
    int wds_number;
    int ppm_number;
    bool blended_position;
    double rah;
    double ram;
    double ras;
    double decd;
    double decm;
    double decs;
    double pmra_arcsec_per_year;
    double pmdec_arcsec_per_year;
    double decsign;
    double pmdecsign;
    std::string fileline;
};

class Skymap {
public:
    explicit Skymap(std::string pathcat);

    std::vector<skymaprec> records;
};

}

#endif //STARID_SKYMAP_H
