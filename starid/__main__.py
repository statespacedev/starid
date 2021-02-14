import argparse

def demo():
    """todo"""
    from libhandler import LibHandler
    from starimage import Starimage
    api = LibHandler()
    img = Starimage(api, targetndx=3)
    img.print()
    img.plot()

if __name__ == '__main__':
    parser = argparse.ArgumentParser('starid')
    parser.add_argument('-d', '--demo', dest='demo', action='store_true', default=False)
    args = parser.parse_args()
    if args.demo: demo()
    else: demo
