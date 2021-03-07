#include "startriangle.h"
#include "util.h"

/*
 * class StartriangleNOMAD:
 *    '''NOMAD triangle. focus is on the basestar and baseside - nomad is about a chain of basesides, each increasing the constraints on the preceding basestars. first constructor here is for the initial triangle and has the target star as basestar. second constructor is for following triangles. each takes side2 from its predecessor and uses that as its baseside. the chain of triangles is a train of baseides - side2 of each triangle is the baseside of the following triangle. during feedback, these shared side2 -> baseside pairs are the path for information to flow backwards, all the way backward from latest triangle to the initial triangle - increasing the constraints on the initial triangle baseside and basestar.'''
 * */
starid::StartriangleNOMAD::StartriangleNOMAD(int basestar, Eigen::MatrixXd &starvecs, starid::Starpairs &starpairs)
    : stara(basestar), starb(basestar), starc(basestar) {
    srand (time(NULL));
    while (starb == stara) starb = rand() % starvecs.rows();
    while (starc == stara || starc == starb) starc = rand() % starvecs.rows();
    side1 = Startriangleside(pairangle(starvecs, stara, starb), starpairs);
    side2 = Startriangleside(pairangle(starvecs, starb, starc), starpairs);
    side3 = Startriangleside(pairangle(starvecs, starc, stara), starpairs);
    this->constrain();
}

starid::StartriangleNOMAD::StartriangleNOMAD(starid::Startriangleside &prevside2, int prevstarb, int prevstarc, Eigen::MatrixXd &starvecs, starid::Starpairs &starpairs)
    : side1(prevside2), stara(prevstarb), starb(prevstarc), starc(prevstarc) {
    while (starc == stara || starc == starb) starc = rand() % starvecs.rows();
    side2 = Startriangleside(pairangle(starvecs, starb, starc), starpairs);
    side3 = Startriangleside(pairangle(starvecs, starc, stara), starpairs);
    this->constrain();
}

/*
 *    def constrain(self):
 *       '''in each of the three sides, there's a pairhalf1 -> pairhalf -> 0 or 1 concept. 0 is the default and means drop this pair. here we will mark pairs to keep by setting them to 1, all others will be dropped.'''
 * */
void starid::StartriangleNOMAD::constrain() {
    for (auto STAR1side1 = side1.stars.begin(), end = side1.stars.end(); STAR1side1 != end; ++STAR1side1) {
        auto STAR1side3 = side3.stars.find(STAR1side1->first); if (STAR1side3 == side3.stars.end()) continue;
        auto &pairs3 = STAR1side3->second; auto &pairs1 = STAR1side1->second;
        for (auto pairs1it = pairs1.begin(), end = pairs1.end(); pairs1it != end; ++pairs1it) {
            auto STAR2side2 = side2.stars.find(pairs1it->first); if (STAR2side2 == side2.stars.end()) continue;
            auto &pairs2 = STAR2side2->second;
            for (auto pairs2it = pairs2.begin(), end = pairs2.end(); pairs2it != end; ++pairs2it) {
                auto STAR3side3 = pairs3.find(pairs2it->first); if (STAR3side3 == pairs3.end()) continue;
                pairs1it->second = 1; pairs2it->second = 1; STAR3side3->second = 1; } } } // keep these pairs
    side1.drops(); side2.drops(); side3.drops(); }

/*
 *    def feedback(self):
 *       '''increase the constraints on the baseside in the prev triangle, using the baseside of the following triangle in the chain. as triangles are added, constraints flow backwards through preceding basesides and basestars. the chain of triangles is a train of baseides - side2 of each triangle is the baseside of the following triangle. during feedback, these shared side2 -> baseside pairs are the path for information to flow backwards, all the way backward from latest triangle to the initial triangle - increasing the constraints on the initial triangle baseside and basestar.'''
 * */
void starid::StartriangleNOMAD::feedback(starid::Startriangleside &nextside1) {
    side2.stars = nextside1.stars;
    this->constrain();
}

/*
 *    def stop(self):
 *       '''stopping condition. true if basestars and basesides have been constrained to the point where only one possible basestar remains.'''
 * */
bool starid::StartriangleNOMAD::stop() {
    if (side1.stars.size() <= 1) return true;
    return false;
}

/*
 * class StartriangleSETTLER:
 *    '''SETTLER triangle. acts as the triangles abca and abda within the star triangle identifier inner loops. their are three triangle sides - representing three star pairs, each with an angular separation. each side is acted by a star triangle side object. star recognition focused on triangles that contain the target star - star a is always the target star, star b is a neighbor star, and an abside is a star pair and triangle side with the target as the first member of the pair. in the inner loops, additional stars c and d are involved. first an abca triangle is formed. this constrains the abside. then for an abca triangle, a sequence of abda triangles are formed, further constraining the abside. when we reach an abda that eliminates all but one star pair possibility for the abside, we've recognized the target star. until that happens, we continue picking new absides, with new abca triangles, with new abda triangles. the name SETTLER comes from the idea that we never move away the target star, we're settling around it.'''
 * */
starid::StartriangleSETTLER::StartriangleSETTLER(double ang1, double ang2, double ang3, starid::Starpairs &pairs, Eigen::Vector3d vecin)
    : side1(ang1, pairs), side2(ang2, pairs), side3(ang3, pairs), vecstar3(vecin) {
    vecstar3 << vecin(0), vecin(1), vecin(2);
}

/*
 *    def constrain_abda(self, triangles):
 *       '''test candidate star pairs for the sides of an abda triangle.'''
 * */
void starid::StartriangleSETTLER::constrain_abda(std::vector<StartriangleSETTLER> &triangles, starid::Starpairs &pairs) {
    int maxtriangles = triangles.size();
    for (int trianglendx = 0; trianglendx < maxtriangles; ++trianglendx) {
        double cdang = acos(vecstar3.transpose() * triangles[trianglendx].vecstar3);
        starid::Startriangleside cd(cdang, pairs);
        loops_cnt = 0;
        for (auto it11 = side1.stars.begin(), end = side1.stars.end(); it11 != end; ++it11) {
            auto &pairs1 = it11->second;
            int star1side1 = it11->first;                     // star1 side1
            auto star1side3 = side3.stars.find(star1side1);   // star1 side3
            if (star1side3 == side3.stars.end()) continue;
            auto &pairs3 = star1side3->second;
            for (auto pairs1it = pairs1.begin(), end = pairs1.end(); pairs1it != end; ++pairs1it) {
                int star2side1 = pairs1it->first;               // star2 side1
                auto star2side2 = side2.stars.find(star2side1); // star2 side2
                if (star2side2 == side2.stars.end()) continue;
                auto &pairs2 = star2side2->second;
                for (auto pairs2it = pairs2.begin(), end = pairs2.end(); pairs2it != end; ++pairs2it) {
                    int star3side2 = pairs2it->first;             // star3 side2
                    auto star3side3 = pairs3.find(star3side2);    // star3 side3
                    if (star3side3 == pairs3.end()) continue;
                    bool dok = false; // is this d star possible
                    auto cdd = cd.stars.find(star3side2);
                    auto aca = triangles[trianglendx].side3.stars.find(star1side1);
                    if (cdd != cd.stars.end() && aca != triangles[trianglendx].side3.stars.end()) {
                        auto &pairscdc = cdd->second;
                        auto &pairsacc = aca->second;
                        for (auto pairscdcit = pairscdc.begin(), end = pairscdc.end();
                             pairscdcit != end; ++pairscdcit) {
                            auto pairsaccit = pairsacc.find(pairscdcit->first);
                            if (pairsaccit != pairsacc.end()) {
                                dok = true;
                                break; } } }
                    if (!dok) continue;
                    pairs1it->second = 1;
                    pairs2it->second = 1;
                    star3side3->second = 1;
                    ++loops_cnt; } } }
        side1.drops(); side2.drops(); side3.drops();
        if (side1.stars.size() == 1) break; } }

/*
 *    def constrain_abca(self):
 *       '''test candidate star pairs for the sides of an abca triangle.'''
 * */
void starid::StartriangleSETTLER::constrain_abca() {
    loops_cnt = 0;
    for (auto it11 = side1.stars.begin(), end = side1.stars.end(); it11 != end; ++it11) {
        auto &pairs1 = it11->second;
        int star1side1 = it11->first;                     // star1 side1
        auto star1side3 = side3.stars.find(star1side1);   // star1 side3
        if (star1side3 == side3.stars.end()) continue;
        auto &pairs3 = star1side3->second;
        for (auto pairs1it = pairs1.begin(), end = pairs1.end(); pairs1it != end; ++pairs1it) {
            int star2side1 = pairs1it->first;               // star2 side1
            auto star2side2 = side2.stars.find(star2side1); // star2 side2
            if (star2side2 == side2.stars.end()) continue;
            auto &pairs2 = star2side2->second;
            for (auto pairs2it = pairs2.begin(), end = pairs2.end(); pairs2it != end; ++pairs2it) {
                int star3side2 = pairs2it->first;             // star3 side2
                auto star3side3 = pairs3.find(star3side2);    // star3 side3
                if (star3side3 == pairs3.end()) continue;
                pairs1it->second = 1;
                pairs2it->second = 1;
                star3side3->second = 1;
                ++loops_cnt; } } }
    side1.drops(); side2.drops(); side3.drops(); }