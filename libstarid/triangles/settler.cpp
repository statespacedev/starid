#include "settler.h"
#include "../sky/geometry.h"

SETTLER::SETTLER(Starpairs &starpairs) : starpairs(starpairs) {}

int SETTLER::run(MatrixXd &pixels) {
    starvecs = pixels_to_starvecs(pixels);
    for (ndxb = 1; ndxb < starvecs.rows(); ++ndxb) { // absides
        uveca = starvecs.row(0);
        uvecb = starvecs.row(ndxb);
        int prev_stars = 0;
        int repeatcnt = 0;
        bool converged = false;
        Startriangleside abside(std::acos(uveca.transpose() * uvecb), starpairs); // abside to investigate

        for (ndxc = 1; ndxc < starvecs.rows(); ++ndxc) { // abca triangles
            if (converged || !get_angs_c()) continue;
            std::vector<StartriangleSETTLER> triangles;
            StartriangleSETTLER abca(angs_c[0], angs_c[1], angs_c[2], starpairs, starvecs.row(ndxc).transpose());
            abca.side1.stars = abside.stars;
            abca.constrain_abca();
            abside.update(abca.side1);
            triangles.push_back(abca);

            for (ndxd = 1; ndxd < starvecs.rows(); ++ndxd) { // abda triangles
                if (converged || !get_angs_d()) continue;
                StartriangleSETTLER abda(angs_d[0], angs_d[4], angs_d[3], starpairs, starvecs.row(ndxd).transpose());
                abda.side1.stars = abside.stars;
                abda.constrain_abda(triangles, starpairs);
                abside.update(abda.side1);
                triangles.push_back(abda);

                if (prev_stars == abside.stars.size()) ++repeatcnt; else repeatcnt = 0;
                if (repeatcnt > 3) converged = true;
                prev_stars = abside.stars.size();
                if (abside.stars.size() == 1) break;
            } // abda stop
            if (abside.stars.size() == 1) break;
        } // abca stop
        if (abside.stars.size() == 1) {
            auto starsit = abside.stars.begin();
            return starsit->first;
        } // only one candidate abside star pair remains
        absides.push_back(abside);
    }
    return -1;
}

bool SETTLER::get_angs_d() {
    if (ndxd == ndxb || ndxd == ndxc) return false;
    bool angsok = true;
    angs_d = angs_c;
    uvecd = starvecs.row(ndxd);
    angs_d.push_back(std::acos(uvecd.transpose() * uveca));
    angs_d.push_back(std::acos(uvecd.transpose() * uvecb));
    angs_d.push_back(std::acos(uvecd.transpose() * uvecc));
    if (angs_d[3] < min_ang) angsok = false; // da
    if (angs_d[4] < min_ang) angsok = false; // db
    if (angs_d[5] < min_ang) angsok = false; // dc
    if (std::abs(angs_d[4] - angs_d[3]) < min_ang) angsok = false; // db-da
    //if (std::abs(angs_d[4]-angs_d[0]) < min_ang) angsok = false; // db-ab
    //if (std::abs(angs_d[4]-angs_d[5]) < min_ang) angsok = false; // db-dc
    return angsok;
}

bool SETTLER::get_angs_c() {
    min_ang = 3000.0 * arcseconds_to_radians;
    if (ndxc == ndxb) return false;
    bool angsok = true;
    angs_c.clear();
    uvecc = starvecs.row(ndxc);
    angs_c.push_back(std::acos(uveca.transpose() * uvecb));
    angs_c.push_back(std::acos(uvecb.transpose() * uvecc));
    angs_c.push_back(std::acos(uvecc.transpose() * uveca));
    if (angs_c[0] < min_ang) angsok = false; // ab
    if (angs_c[1] < min_ang) angsok = false; // bc
    if (angs_c[2] < min_ang) angsok = false; // ca
    if (std::abs(angs_c[0] - angs_c[1]) < min_ang) angsok = false; // ab-bc
    if (std::abs(angs_c[0] - angs_c[2]) < min_ang) angsok = false; // ab-ca
    if (std::abs(angs_c[1] - angs_c[2]) < min_ang) angsok = false; // bc-ca
    return angsok;
};

