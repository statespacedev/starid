#include "libstarid.h"

libstarid::libstarid() {
}

void libstarid::load_sky() {
    std::string skydata = "/home/noah/starid/data/";
    std::ifstream is1(std::string(skydata + "sky"));
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(sky);
}

Eigen::MatrixXd libstarid::image_info(int starndx) {
    Eigen::MatrixXd image_info = starid::pointing_vectors::new_image_info(starndx, sky);
    return image_info;
}

Eigen::MatrixXd libstarid::image(int starndx) {
    Eigen::MatrixXd img = starid::pointing_vectors::new_image_matrix(starndx, sky);
    return img;
}

//void libstarid::load_pairs() {
//    std::string pairsdata = "/home/noah/starid/identification/";
//    std::ifstream is2(std::string(pairsdata + "pairs"));
//    cereal::BinaryInputArchive iarchive2(is2);
//    iarchive2(pairs);
//}

//int libstarid::id(starid::image_matrix imgmat) {
//    starid::triangles ider(imgmat, pairs);
//    int result = ider.id();
//    return result;
//}

//Eigen::MatrixXd libstarid::ang_seq_vec(int starndx) {
//    Eigen::MatrixXd angseqvec = starid::pointing_vectors::new_ang_seq_vec(starndx, sky);
//    return angseqvec;
//}
