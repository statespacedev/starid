import argparse, sys, os
sys.path.append('starid')
sys.path.append('cmake-build-debug/libstarid')

dirdata = os.getcwd() + '/data/'

def main():
    parser = argparse.ArgumentParser('starid')
    parser.add_argument('-w', '--wsky', help='write sky binary file', action='store_true')
    parser.add_argument('-r', '--rsky', help='read sky binary file', action='store_true')
    parser.add_argument('--sky', help='filename for sky binary file', dest='sky', action='store', type=str)
    parser.add_argument('--cat', help='filename for skymap text file', dest='cat', action='store', type=str)
    args = parser.parse_args()

    if args.rsky:
        pathsky = dirdata + 'sky'
        if args.sky:
            pathsky = dirdata + args.sky
        if not os.path.exists(pathsky):
            print('file check unsuccessful, %s' % (pathsky))
            return
        import util
        util.read_sky(pathsky)

    if args.wsky:
        pathcat = dirdata + 'cat'
        if args.cat:
            pathcat = dirdata + args.cat
        if not os.path.exists(pathcat):
            print('file check unsuccessful, %s' % (pathcat))
            return
        pathsky = dirdata + 'sky'
        if args.sky:
            pathsky = dirdata + args.sky
        import util
        util.write_sky(pathcat, pathsky)

if __name__ == "__main__":
    main()
