import argparse, sys
sys.path.append('starid')

def main():
    parser = argparse.ArgumentParser('starid')
    parser.add_argument('-t', help='test the project', action='store_true')
    args = parser.parse_args()

    if args.t:
        import util
        util.image()

if __name__ == "__main__":
    main()
