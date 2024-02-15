"""entrypoint for pure python workflows. intended as 'hello world', bare minimum, first thing to get running. also
intended to be directly comparable with libstatid-debug/entrypoint_main.cpp"""
import os
import pickle
from starid.util.get_project_root import get_project_root
from starid.sky.sky import Sky

dirdata = get_project_root() + '/data/'
dirtmp = get_project_root() + '/tmp/'
pathskymap = dirdata + "skymap.txt"
pathsky = dirtmp + "sky.pickle"
pathstarpairs = dirtmp + "starpairs.pickle"
if not os.path.exists(dirtmp): os.makedirs(dirtmp)

def main():
    """directly comparable with main() of libstatid-debug/entrypoint_main.cp"""
    sky = sky_util(pathsky, pathskymap)
    sky.show_image_of_target_star(3)

    return

def sky_util(pathsky_, pathskymap_):
    """load sky object from disk. if not present, create and write."""
    try:
        sky = pickle.load(open(pathsky_, 'rb'))
    except (Exception,):
        sky = Sky()
        sky.start(pathskymap_)
        pickle.dump(sky, open(pathsky_, 'wb'))
    return sky


if __name__ == '__main__':
    main()
