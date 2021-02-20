#include "startrianglestarid.h"
#include "startriangle.h"

/*
 * class Startriangles:
 *    '''work with the stars as triangles.'''
 * */
starid::StartriangleStarid::StartriangleStarid(Starpairs &starpairs) : starpairs(starpairs) {
    double epsilon = 0.0;
    tolerance = (2.0 * std::sqrt(500.0 * 500.0 + 500.00 * 500.0) + epsilon) * starid::arcseconds_to_radians;
}

/*
 *    def identify(self, pixels, teststar):
 *       ''' '''
 * */
int starid::StartriangleStarid::identify(Eigen::MatrixXd &pixels, int teststar) {
    starvecs = starid::pixels_to_starvecs(pixels);
    std::vector<Startriangleside> abs;
    for (ndxb = 1; ndxb < starvecs.rows(); ++ndxb) {
        uveca = starvecs.row(0);
        uvecb = starvecs.row(ndxb);
        Startriangleside ab(std::acos(uveca.transpose() * uvecb), tolerance, starpairs, teststar);
        int prev_stars = 0;
        int repeatcnt = 0;
        bool converged = false;
        for (ndxc = 1; ndxc < starvecs.rows(); ++ndxc) {
            if (converged || !get_angs_c()) continue;
            Startriangle abca(angs_c[0], angs_c[1], angs_c[2], tolerance, starpairs, teststar, starvecs.row(ndxc).transpose());
            abca.side1.stars = ab.stars;
            abca.close_loops_abca();
            ab.append_iterations(abca.side1);
            std::vector<Startriangle> triangles;
            triangles.push_back(abca);
            for (ndxd = 1; ndxd < starvecs.rows(); ++ndxd) {
                if (converged || !get_angs_d()) continue;
                Startriangle abda(angs_d[0], angs_d[4], angs_d[3], tolerance, starpairs, teststar, starvecs.row(ndxd).transpose());
                abda.side1.stars = ab.stars;
                abda.close_loops_abda(triangles);
                ab.append_iterations(abda.side1);
                triangles.push_back(abda);
                if (prev_stars == ab.stars.size()) ++repeatcnt; else repeatcnt = 0;
                if (repeatcnt > 3) converged = true;
                prev_stars = ab.stars.size();
                if (ab.stars.size() == 1) break;
            }
            if (ab.stars.size() == 1) break;
        }
        if (ab.stars.size() == 1) {
            auto starsit = ab.stars.begin();
            return starsit->first;
        }
        abs.push_back(ab);
    }
    return -1;
}

/*
 *    def get_angs_d(self):
 *       ''' '''
 * */
bool starid::StartriangleStarid::get_angs_d() {
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

/*
 *    def get_angs_c(self):
 *       ''' '''
 * */
bool starid::StartriangleStarid::get_angs_c() {
    if (ndxc == ndxb) return false;
    bool angsok = true;
    angs_c.clear();
    uvecc = starvecs.row(ndxc);
    angs_c.push_back(std::acos(uveca.transpose() * uvecb));
    angs_c.push_back(std::acos(uvecb.transpose() * uvecc));
    angs_c.push_back(std::acos(uvecc.transpose() * uveca));
    min_ang = 3000.0 * starid::arcseconds_to_radians;
    if (angs_c[0] < min_ang) angsok = false; // ab
    if (angs_c[1] < min_ang) angsok = false; // bc
    if (angs_c[2] < min_ang) angsok = false; // ca
    if (std::abs(angs_c[0] - angs_c[1]) < min_ang) angsok = false; // ab-bc
    if (std::abs(angs_c[0] - angs_c[2]) < min_ang) angsok = false; // ab-ca
    if (std::abs(angs_c[1] - angs_c[2]) < min_ang) angsok = false; // bc-ca
    return angsok;
};

