import argparse, demo

if __name__ == "__main__":
    parser = argparse.ArgumentParser('starid')
    parser.add_argument('-d', '--demo', help='demo star image', action='store_true')
    args = parser.parse_args()
    demo.demo(args)

