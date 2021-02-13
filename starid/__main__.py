import sys, os, argparse
sys.path.append('build/cmake/libstarid')
import libstarid

class Config():
    def __init__(self, args=None):
        self.cwd = os.getcwd() # expect */starid, not */starid/starid
        self.args = args
        self.dirsky = './data/'
        self.namecat = 'cat'
        self.namesky = 'sky'
        self.ls = libstarid.libstarid()
        if not os.path.exists(self.dirsky + self.namesky):
            self.ls.write_sky(self.dirsky + self.namesky, self.dirsky + self.namecat)
        self.ls.read_sky(self.dirsky + self.namesky)

def demo(conf, targetndx):
    from starimage import Starimg
    img = Starimg(conf, targetndx)
    img.print_starlist()
    img.plot_image()

if __name__ == "__main__":
    parser = argparse.ArgumentParser('starid')
    parser.add_argument('-d', '--demo', help='demo star image', action='store_true')
    args = parser.parse_args()
    conf = Config(args)
    if conf.args.demo: demo(conf, targetndx=3)
