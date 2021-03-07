#include "startriangleidentifier.h"
#include "util.h"

/*
 * class NOMAD:
 *    '''star recognition focused on a chain of triangles and basesides - side2 of each triangle is the baseside of the following triangle. during feedback, these shared side2 -> baseside pairs are the path for information to flow backwards - increasing the constraints on the initial triangle baseside and basestar. the name NOMAD relates to how the chain of triangles wanders away from the target star and initial triangle.'''
 * */
starid::NOMAD::NOMAD(Starpairs &starpairs) : starpairs(starpairs), maxtriangles(90) {}

/*
 *    def run(self, pixels):
 *       '''recognize target star from the image pixels.'''
 * */
int starid::NOMAD::run(Eigen::MatrixXd &pixels) {
    starvecs = pixels_to_starvecs(pixels);
    triangles.emplace_back(StartriangleNOMAD(0, starvecs, starpairs)); int cnt = 1;
    while (cnt < maxtriangles && !triangles[0].stop()) {
        triangles.emplace_back(StartriangleNOMAD(triangles.back().side2, triangles.back().starb, triangles.back().starc, starvecs, starpairs));
        for (int ndx = (int)triangles.size() - 1; ndx > 0; --ndx) triangles[ndx - 1].feedback(triangles[ndx].side1);
        ++cnt; }
    return -1;
}

/*
 * class SETTLER:
 *    '''the target star ia always star a. star b is a neighbor star, and an abside is a star pair and triangle side with the target as the first member of the pair. in the inner loops, additional stars c and d are involved. first an abca triangle is formed. this constrains the abside. then for an abca triangle, a sequence of abda triangles are formed, further constraining the abside. when we reach an abda that eliminates all but one star pair possibility for the abside, we've recognized the target star. the name SETTLER comes from the idea that we never move away the target star, we're settling around it.'''
 * */
starid::SETTLER::SETTLER(Starpairs &starpairs) : starpairs(starpairs) {}

/*
 *    def run(self, pixels):
 *       '''recognize target star from the image pixels.'''
 * */
int starid::SETTLER::run(Eigen::MatrixXd &pixels) {
    starvecs = pixels_to_starvecs(pixels); std::vector<Startriangleside> absides;

    for (ndxb = 1; ndxb < starvecs.rows(); ++ndxb) { // absides
        uveca = starvecs.row(0); uvecb = starvecs.row(ndxb); int prev_stars = 0; int repeatcnt = 0; bool converged = false;
        Startriangleside abside(std::acos(uveca.transpose() * uvecb), starpairs); // abside to investigate

        for (ndxc = 1; ndxc < starvecs.rows(); ++ndxc) { // abca triangles
            if (converged || !get_angs_c()) continue; std::vector<StartriangleSETTLER> triangles;
            StartriangleSETTLER abca(angs_c[0], angs_c[1], angs_c[2], starpairs, starvecs.row(ndxc).transpose());
            abca.side1.stars = abside.stars;
            abca.constrain_abca(); abside.update(abca.side1);
            triangles.push_back(abca);

            for (ndxd = 1; ndxd < starvecs.rows(); ++ndxd) { // abda triangles
                if (converged || !get_angs_d()) continue;
                StartriangleSETTLER abda(angs_d[0], angs_d[4], angs_d[3], starpairs, starvecs.row(ndxd).transpose());
                abda.side1.stars = abside.stars;
                abda.constrain_abda(triangles, starpairs); abside.update(abda.side1);
                triangles.push_back(abda);

                if (prev_stars == abside.stars.size()) ++repeatcnt; else repeatcnt = 0;
                if (repeatcnt > 3) converged = true;
                prev_stars = abside.stars.size();
                if (abside.stars.size() == 1) break; } // abda stop
            if (abside.stars.size() == 1) break; } // abca stop
        if (abside.stars.size() == 1) { auto starsit = abside.stars.begin(); return starsit->first; } // only one candidate abside star pair remains
        absides.push_back(abside); }
    return -1; }

/*
 *    def get_angs_d(self):
 *       '''examine a candidate for star d before using it to form triangle abda. we want the angles from stars a, b, and c to be appreciable. the angles remain in angs_d for later use'''
 * */
bool starid::SETTLER::get_angs_d() {
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
 *       '''examine a candidate for star c before using it to form triangle abca. we want the angles between stars a, b, and c to be appreciable. the angles remain in angs_c for later use.'''
 * */
bool starid::SETTLER::get_angs_c() {
    min_ang = 3000.0 * starid::arcseconds_to_radians;
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

