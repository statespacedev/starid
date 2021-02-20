#include "startriangleidentifier.h"
#include "startriangle.h"

/*
 * class StartriangleIdentifier:
 *    '''identifies the target of a star image, using the triangles formed by neighboring stars within the image. the fundemental particles are actually pairs of stars - in a sense individual stars don't exist here, what exists are pairs of stars, acting as sides of triangles - so a key object handed to the identifier in its constructor is a starpairs object, containing all of the relevant pairs. when possible, the starpairs object was loaded from a cerealized starpairs file, rather than generated at run-time.'''
 * */
starid::StartriangleIdentifier::StartriangleIdentifier(Starpairs &starpairs) : starpairs(starpairs) {
    double epsilon = 0.0;
    tolerance = (2.0 * std::sqrt(500.0 * 500.0 + 500.00 * 500.0) + epsilon) * starid::arcseconds_to_radians;
}

/*
 *    def identify(self, pixels, teststar):
 *       '''recognizes the target of a star image, received as image pixels. the focus is on triangles that contain the target star - these are the absides, where star a is the target star, star b is a neighbor star, and an abside is a star pair and triangle side with the target as the first member of the pair. in the inner loops, two additional stars are involved - star c and star d. they form two star triangles - triangle abca, and triangle abda. when we reach an abside such that abca and abda eliminate all but one star pair possibility, we've recognized the target star.'''
 * */
int starid::StartriangleIdentifier::identify(Eigen::MatrixXd &pixels, int teststar) {
    starvecs = starid::pixels_to_starvecs(pixels); std::vector<Startriangleside> absides;

    for (ndxb = 1; ndxb < starvecs.rows(); ++ndxb) {
        uveca = starvecs.row(0); uvecb = starvecs.row(ndxb); int prev_stars = 0; int repeatcnt = 0; bool converged = false;
        Startriangleside abside(std::acos(uveca.transpose() * uvecb), tolerance, starpairs, teststar); // abside to investigate

        for (ndxc = 1; ndxc < starvecs.rows(); ++ndxc) {
            if (converged || !get_angs_c()) continue; std::vector<Startriangle> triangles;
            Startriangle abca(angs_c[0], angs_c[1], angs_c[2], tolerance, starpairs, teststar, starvecs.row(ndxc).transpose());
            abca.side1.stars = abside.stars; abca.close_loops_abca();
            abside.append_iterations(abca.side1); triangles.push_back(abca);

            for (ndxd = 1; ndxd < starvecs.rows(); ++ndxd) {
                if (converged || !get_angs_d()) continue;
                Startriangle abda(angs_d[0], angs_d[4], angs_d[3], tolerance, starpairs, teststar, starvecs.row(ndxd).transpose());
                abda.side1.stars = abside.stars; abda.close_loops_abda(triangles);
                abside.append_iterations(abda.side1); triangles.push_back(abda);

                if (prev_stars == abside.stars.size()) ++repeatcnt; else repeatcnt = 0;
                if (repeatcnt > 3) converged = true;
                prev_stars = abside.stars.size();
                if (abside.stars.size() == 1) break; }
            if (abside.stars.size() == 1) break; }
        if (abside.stars.size() == 1) { auto starsit = abside.stars.begin(); return starsit->first; } // only one abside star pair remains
        absides.push_back(abside); }
    return -1; }

/*
 *    def get_angs_d(self):
 *       ''' '''
 * */
bool starid::StartriangleIdentifier::get_angs_d() {
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
bool starid::StartriangleIdentifier::get_angs_c() {
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

