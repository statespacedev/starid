#include <fstream>
#include "sky.h"
#include "skymap.h"

/*
 * class Skymap:
 *    '''bring the nasa skymap sky2000 v5r4 star catalog in. there are peculiarities to this catalog, and they should be reflected in its representation here. briefly, v5r4 was targeted at real world star tracker users - it tried to fuse results from multiple predecessor catalogs to provide useful information.'''
 * */
starid::Skymap::Skymap(std::string pathcat) {
    std::ifstream catfile(pathcat);
    if (catfile.is_open()) {
        std::string line;
        skymaprec rec;
        while (std::getline(catfile, line)) {
            try {
                try { rec.mv1 = std::stof(line.substr(232, 6)); } catch (...) {}
                if (rec.mv1 > starid::star_brightness_limit) continue;
                rec.iau_identifier = line.substr(0, 27);
                try { rec.star_name = line.substr(98, 10); } catch (...) {}
                try { rec.variablestar_name = line.substr(108, 10); } catch (...) {}
                rec.skymap_number = std::stoi(line.substr(27, 8));
                try { rec.hd_number = std::stoi(line.substr(35, 8)); } catch (...) {}
                try { rec.sao_number = std::stoi(line.substr(43, 7)); } catch (...) {}
                try { rec.dm_number = std::stoi(line.substr(50, 13)); } catch (...) {}
                try { rec.hr_number = std::stoi(line.substr(63, 4)); } catch (...) {}
                try { rec.wds_number = std::stoi(line.substr(67, 6)); } catch (...) {}
                try { rec.ppm_number = std::stoi(line.substr(83, 7)); } catch (...) {}
                try { rec.blended_position = std::stoi(line.substr(146, 1)); } catch (...) {}
                rec.rah = std::stof(line.substr(118, 2));
                rec.ram = std::stof(line.substr(120, 2));
                rec.ras = std::stof(line.substr(122, 7));
                rec.decd = std::stof(line.substr(130, 2));
                rec.decm = std::stof(line.substr(132, 2));
                rec.decs = std::stof(line.substr(134, 6));
                rec.pmra_arcsec_per_year = 15.0 * std::stof(line.substr(149, 8));
                rec.pmdec_arcsec_per_year = std::stof(line.substr(158, 7));
                rec.decsign = 1.0;
                rec.pmdecsign = 1.0;
                if (line.substr(129, 1).compare("-") == 0) rec.decsign = -1.0;
                if (line.substr(157, 1).compare("-") == 0) rec.pmdecsign = -1.0;
                rec.fileline = line;
                records.push_back(rec);
            } catch (...) {
            }
        }
        catfile.close();
    }
}