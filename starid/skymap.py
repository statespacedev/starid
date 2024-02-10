from starid.definitions import star_brightness_limit

class Skymap:
    """bring the nasa skymap sky2000 v5r4 star catalog in. there are peculiarities to this catalog, and they should
    be reflected in its representation here. briefly, v5r4 was targeted at real world star tracker users - it tried
    to fuse results from multiple predecessor catalogs to provide useful information."""

    def __init__(self, pathskymap):
        self.records = []
        for line in open(pathskymap, 'rt'):
            rec = Rec()
            rec.fileline = line
            rec.mv1 = float(line[232:238])
            if rec.mv1 > star_brightness_limit: continue
            rec.iau_identifier = line[0:27].strip()
            rec.star_name = line[98:108].strip()
            rec.variablestar_name = line[108:118].strip()
            rec.skymap_number = int(line[27:35])
            rec.hd_number = int(line[35:43]) if line[35:43].strip() else None
            rec.sao_intnumber = int(line[43:50]) if line[43:50].strip() else None
            rec.dm_number = line[50:63].strip()
            rec.hr_number = int(line[63:67]) if line[63:67].strip() else None
            # try { rec.wds_number = std::stoi(line.substr(67, 6)); } catch (...) {}
            # try { rec.ppm_number = std::stoi(line.substr(83, 7)); } catch (...) {}
            # try { rec.blended_position = std::stoi(line.substr(146, 1)); } catch (...) {}
            # rec.rah = std::stof(line.substr(118, 2));
            # rec.ram = std::stof(line.substr(120, 2));
            # rec.ras = std::stof(line.substr(122, 7));
            # rec.decd = std::stof(line.substr(130, 2));
            # rec.decm = std::stof(line.substr(132, 2));
            # rec.decs = std::stof(line.substr(134, 6));
            # rec.pmra_arcsec_per_year = 15.0 * std::stof(line.substr(149, 8));
            # rec.pmdec_arcsec_per_year = std::stof(line.substr(158, 7));
            # rec.decsign = 1.0;
            # rec.pmdecsign = 1.0;
            # if (line.substr(129, 1).compare("-") == 0) rec.decsign = -1.0;
            # if (line.substr(157, 1).compare("-") == 0) rec.pmdecsign = -1.0;
            self.records.append([rec])

class Rec:
    fileline = None
