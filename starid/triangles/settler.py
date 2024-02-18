"""the target star ia always star a. star b is a neighbor star, and an abside is a star pair and
triangle side with the target as the first member of the pair. in the inner loops, additional stars c and d are
involved. first an abca triangle is formed. this constrains the abside. then for an abca triangle, a sequence of
abda triangles are formed, further constraining the abside. when we reach an abda that eliminates all but one
star pair possibility for the abside, we've recognized the target star. the name SETTLER comes from the idea that
we never move away the target star, we're settling around it."""

class SETTLER:
    """recognize target star from triangles where the target star is always star a."""

    def __init__(self, starpairs):
        self.starpairs = starpairs

    def run(self, image):
        """recognize target star from the image pixels."""
        starvecs, absides = image.img_to_pvecs(), []
        for starvec in starvecs:
            pass

    # std::vector<Startriangleside> absides;
    # for (ndxb = 1; ndxb < starvecs.rows(); ++ndxb) { // absides
    #     uveca = starvecs.row(0);
    #     uvecb = starvecs.row(ndxb);
    #     int prev_stars = 0;
    #     int repeatcnt = 0;
    #     bool converged = false;
    #     Startriangleside abside(std::acos(uveca.transpose() * uvecb), starpairs); // abside to investigate


    def get_angs_d(self):
        """examine a candidate for star d before using it to form triangle abda. we want the angles from stars a, b,
        and c to be appreciable. the angles remain in angs_d for later use"""

    def get_angs_c(self):
        """examine a candidate for star c before using it to form triangle abca. we want the angles between stars a,
        b, and c to be appreciable. the angles remain in angs_c for later use."""
