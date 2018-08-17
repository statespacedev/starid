import argparse, sys
sys.path.append('starid')
sys.path.append('cmake-build-debug/libstarid')

def main():
    parser = argparse.ArgumentParser('starid')
    parser.add_argument('-t', help='test the project, shows a star image', action='store_true')
    args = parser.parse_args()

    if args.t:
        import util
        util.image()

if __name__ == "__main__":
    main()
