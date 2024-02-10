from starid.sky import Sky

# use approprate path formatting, on windows C:\\Users\\i28144\\CLionProjects\\starid\\data\\skymap.txt
dirdata = "C:\\Users\\i28144\\CLionProjects\\starid\\data\\"
dirtmp = "C:\\Users\\i28144\\CLionProjects\\starid\\tmp\\"
pathskymap = dirdata + "skymap.txt"
pathsky = dirtmp + "sky.pickle"
pathstarpairs = dirtmp + "starpairs.pickle"

def main():
    sky = Sky()
    sky.start(pathskymap)
    return

if __name__ == '__main__':
    main()
