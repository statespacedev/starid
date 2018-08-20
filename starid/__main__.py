import argparse, sys
sys.path.append('starid')
sys.path.append('cmake-build-debug/libstarid')

def main():
    parser = argparse.ArgumentParser('starid')
    parser.add_argument('-w', '--wsky', help='write sky binary file', action='store_true')
    parser.add_argument('-r', '--rsky', help='read sky binary file and show a star image', action='store_true')
    args = parser.parse_args()

    if args.readsky:
        import util
        util.read_sky()

    if args.writesky:
        import util
        util.write_sky()

if __name__ == "__main__":
    main()
