import pprint
from starid.definitions import star_brightness_limit

class Skymap:
    """bring the nasa skymap sky2000 v5r4 star catalog in. there are peculiarities to this catalog, and they should
    be reflected in its representation here. briefly, v5r4 was targeted at real world star tracker users - it tried
    to fuse results from multiple predecessor catalogs to provide useful information."""

    def __init__(self, pathskymap):
        self.records = []
        for line in open(pathskymap, 'rt'):
            rec = Rec()
            # rec.fileline = line
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
            # rec.wds_number = int(line[67:73]) if line[67:73].strip() else None
            rec.ppm_number = int(line[83:90]) if line[83:90].strip() else None
            # rec.blended_position = int(line[146:147]) if line[146:147].strip() else None
            rec.rah = float(line[118:120])
            rec.ram = float(line[120:122])
            rec.ras = float(line[122:129])
            rec.decd = float(line[130:132])
            rec.decm = float(line[132:134])
            rec.decs = float(line[134:140])
            rec.pmra_arcsec_per_year = 15.0 * float(line[149:157])
            rec.pmdec_arcsec_per_year = float(line[158:165])
            rec.decsign = -1.0 if line[129] == '-' else 1.0;
            rec.pmdecsign = -1.0 if line[157] == '-' else 1.0;
            self.records.append(rec)
            # pprint.pprint(vars(rec))

class Rec:
    fileline = None
