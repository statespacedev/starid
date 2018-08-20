import argparse, sys, os
sys.path.append('starid')
sys.path.append('cmake-build-debug/libstarid')

dirdata = os.getcwd() + '/data/'

def main():
    parser = argparse.ArgumentParser('starid')
    parser.add_argument('-w', '--wsky', help='write sky binary file', action='store_true')
    parser.add_argument('-r', '--rsky', help='read sky binary file', action='store_true')
    parser.add_argument('--sky', help='filename for sky binary file', dest='fnsky', action='store', type=str)
    args = parser.parse_args()

    if args.rsky:
        pathsky = dirdata + 'sky'
        if args.fnsky:
            pathsky = dirdata + args.fnsky
        import util
        util.read_sky(pathsky)

    if args.wsky:
        import util
        util.write_sky()

if __name__ == "__main__":
    main()
