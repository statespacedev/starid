import argparse, sys, os
sys.path.append('starid')
sys.path.append('build/lib.linux-x86_64-3.6')

dirdata = os.getcwd() + '/data/'

def main():
    parser = argparse.ArgumentParser('starid')
    parser.add_argument('-t', '--test', help='show test image', action='store_true')
    parser.add_argument('--cat', help='filename for skymap text file', dest='cat', action='store', type=str)
    parser.add_argument('--sky', help='filename for sky binary file', dest='sky', action='store', type=str)
    parser.add_argument('--starpairs', help='filename for starpairs binary file', dest='pairs', action='store', type=str)
    parser.add_argument('--rsky', help='read sky binary file', action='store_true')
    parser.add_argument('--wsky', help='write sky binary file', action='store_true')
    parser.add_argument('--rstarpairs', help='read starpairs binary file', action='store_true')
    parser.add_argument('--wstarpairs', help='write starpairs binary file', action='store_true')
    args = parser.parse_args()

    if args.test:
        import util
        util.test_sky(os.getcwd() + '/data/sky')

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
        util.write_sky(pathsky, pathcat)

    if args.rstarpairs:
        pathstarpairs = dirdata + 'starpairs'
        if args.starpairs:
            pathstarpairs = dirdata + args.starpairs
        if not os.path.exists(pathstarpairs):
            print('file check unsuccessful, %s' % (pathstarpairs))
            return
        import util
        util.read_starpairs(pathstarpairs)

    if args.wstarpairs:
        pathsky = dirdata + 'sky'
        if args.sky:
            pathsky = dirdata + args.sky
        if not os.path.exists(pathsky):
            print('file check unsuccessful, %s' % (pathsky))
            return
        pathstarpairs = dirdata + 'starpairs'
        if args.starpairs:
            pathstarpairs = dirdata + args.starpairs
        import util
        util.write_starpairs(pathstarpairs)

if __name__ == "__main__":
    main()
