import argparse, sys
sys.path.append('starid')

def main():
    parser = argparse.ArgumentParser('starid')
    parser.add_argument('-t', help='shows a test star image', action='store_true')
    args = parser.parse_args()
    if args.t:
        import test
        test.test()

if __name__ == "__main__":
    main()