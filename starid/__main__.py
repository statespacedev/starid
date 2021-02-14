import argparse

def main():
    """todo"""
    from libhandler import LibHandler
    from starimage import Starimage
    api = LibHandler()
    img = Starimage(api, targetndx=3)
    img.print()
    img.plot()

if __name__ == '__main__':
    parser = argparse.ArgumentParser('starid')
    parser.add_argument('-d', '--demo', help='demo star image', action='store_true')
    args = parser.parse_args()
    main()
