#include <fstream>
#include "skymap.h"

starid::Skymap::Skymap(std::string pathskymap) {
    std::ifstream catfile(pathskymap);
    if (catfile.is_open()) {
        std::string line;
        Skymaprec rec;
        while (std::getline(catfile, line)) {
            rec.mv1 = std::stof(line.substr(232, 6));
            if (rec.mv1 > starid::star_brightness_limit) continue;
            rec.iau_identifier = line.substr(0, 27);
            rec.skymap_number = std::stoi(line.substr(27, 8));
            rec.rah = std::stof(line.substr(118, 2));
            rec.ram = std::stof(line.substr(120, 2));
            rec.ras = std::stof(line.substr(122, 7));
            rec.decd = std::stof(line.substr(130, 2));
            rec.decm = std::stof(line.substr(132, 2));
            rec.decs = std::stof(line.substr(134, 6));
            rec.decsign = 1.0;
            if (line.substr(129, 1).compare("-") == 0) rec.decsign = -1.0;
            rec.fileline = line;
            records.push_back(rec);}
        catfile.close();}}
