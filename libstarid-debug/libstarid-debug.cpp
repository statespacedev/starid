#include "util-debug.h"
#include "startriangleidentifier.h"

// use approprate path formatting, on windows C:\\Users\\i28144\\CLionProjects\\starid\\data\\cat
static std::string dirdata = "C:\\Users\\i28144\\CLionProjects\\starid\\data\\";
static std::string dirtmp = "C:\\Users\\i28144\\CLionProjects\\starid\\tmp\\";
static std::string pathcat = dirdata + "cat";
static std::string pathsky = dirtmp + "sky";
static std::string pathstarpairs = dirtmp + "starpairs";

int main() {
    auto sky = read_sky(pathsky, pathcat);
    auto starpairs = read_starpairs(sky, pathstarpairs);
    auto image = sky.image_generator(2);
    starid::SETTLER settler(starpairs);
    auto id1 = settler.run(image["pixels"]);
    starid::NOMAD nomad(starpairs);
    auto id2 = nomad.run(image["pixels"]);
    return 0;
}

