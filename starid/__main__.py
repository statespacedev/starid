import argparse


def demo():
    from starid.demo import Demo
    demo = Demo()
    demo.plot(starndx=3)


if __name__ == '__main__':
    parser = argparse.ArgumentParser('starid')
    parser.add_argument('-d', '--demo', dest='demo', action='store_true', default=False)
    args = parser.parse_args()
    if args.demo: demo()
