"""entrypoint for pure python workflows. intended as 'hello world', bare minimum, first thing to get running."""
from starid.sky.sky import Sky

# use approprate path formatting, on windows C:\\Users\\i28144\\CLionProjects\\starid\\data\\skymap.txt
dirdata = "C:\\Users\\i28144\\CLionProjects\\starid\\data\\"
dirtmp = "C:\\Users\\i28144\\CLionProjects\\starid\\tmp\\"
pathskymap = dirdata + "skymap.txt"
pathsky = dirtmp + "sky.pickle"
pathstarpairs = dirtmp + "starpairs.pickle"

def main():
    starid = Starid()
    starid.sky.show_image_of_target_star(13)
    return

class Starid:
    """entrypoint object for using pure python workflows. will try to keep everything flowing from here separate from
    both python extension in cpp, and pure cpp."""

    def __init__(self):
        self.sky = Sky()
        self.sky.start(pathskymap)
        pass


if __name__ == '__main__':
    main()
