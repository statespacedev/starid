#ifndef STARID_SKYMAP_H
#define STARID_SKYMAP_H

#include "geometry.h"

namespace starid {
    struct Skymaprec {
        double mv1;
        std::string iau_identifier;
        int skymap_number;
        double rah;
        double ram;
        double ras;
        double decd;
        double decm;
        double decs;
        double decsign;
        std::string fileline;
    };

    class Skymap {
    public:
        explicit Skymap(std::string pathskymap);
        std::vector<Skymaprec> records;
    };
}

#endif //STARID_SKYMAP_H
