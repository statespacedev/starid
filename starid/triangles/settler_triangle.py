"""SETTLER triangle. acts as the triangles abca and abda within the star triangle identifier inner loops. their are
three triangle sides - representing three star pairs, each with an angular separation. each side is acted by a star
triangle side object. star recognition focused on triangles that contain the target star - star a is always the
target star, star b is a neighbor star, and an abside is a star pair and triangle side with the target as the first
member of the pair. in the inner loops, additional stars c and d are involved. first an abca triangle is formed. this
constrains the abside. then for an abca triangle, a sequence of abda triangles are formed, further constraining the
abside. when we reach an abda that eliminates all but one star pair possibility for the abside, we've recognized the
target star. until that happens, we continue picking new absides, with new abca triangles, with new abda triangles.
the name SETTLER comes from the idea that we never move away the target star, we're settling around it."""
from starid.triangles.star_triangle_side import StarTriangleSide

class SETTLERTriangle:
    """acts as the triangles abca and abda within the star triangle identifier inner loops. their are three triangle
    sides - representing three star pairs, each with an angular separation."""

    def __init__(self, sv, ang1, ang2, ang3, starpairs):
        self.side1 = StarTriangleSide(ang1, starpairs)
        self.side2 = StarTriangleSide(ang2, starpairs)
        self.side3 = StarTriangleSide(ang3, starpairs)
        self.vecstar3 = sv
        pass

    def constrain_abca(self):
        """test candidate star pairs for the sides of an abca triangle."""
        loops_cnt = 0
        # for (auto it11 = side1.stars.begin(), end = side1.stars.end(); it11 != end; ++it11) {
        #     auto &pairs1 = it11->second;
        #     int star1side1 = it11->first;                     // star1 side1
        #     auto star1side3 = side3.stars.find(star1side1);   // star1 side3
        #     if (star1side3 == side3.stars.end()) continue;
        #     auto &pairs3 = star1side3->second;
        #     for (auto pairs1it = pairs1.begin(), end = pairs1.end(); pairs1it != end; ++pairs1it) {
        #         int star2side1 = pairs1it->first;               // star2 side1
        #         auto star2side2 = side2.stars.find(star2side1); // star2 side2
        #         if (star2side2 == side2.stars.end()) continue;
        #         auto &pairs2 = star2side2->second;
        #         for (auto pairs2it = pairs2.begin(), end = pairs2.end(); pairs2it != end; ++pairs2it) {
        #             int star3side2 = pairs2it->first;             // star3 side2
        #             auto star3side3 = pairs3.find(star3side2);    // star3 side3
        #             if (star3side3 == pairs3.end()) continue;
        #             pairs1it->second = 1;
        #             pairs2it->second = 1;
        #             star3side3->second = 1;
        #             ++loops_cnt;
        #         }
        #     }
        # }
        # side1.drops();
        # side2.drops();
        # side3.drops();

    def constrain_abda(self, triangles):
        """test candidate star pairs for the sides of an abda triangle."""
        pass
