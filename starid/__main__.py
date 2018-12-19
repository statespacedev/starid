import sys, os
from config import Config
sys.path.append('starid')
sys.path.append('build/lib.linux-x86_64-3.6')

dirdata = os.getcwd() + '/data/'

def rsky(conf):
    pathsky = dirdata + 'sky'
    if conf.sky:
        pathsky = dirdata + args.sky
    if not os.path.exists(pathsky):
        print('file check unsuccessful, %s' % (pathsky))
        return
    import libstarid
    ls = libstarid.libstarid()
    ls.read_sky(conf.pathsky)

def wsky(conf):
    pathcat = dirdata + 'cat'
    if conf.cat:
        pathcat = dirdata + conf.cat
    if not os.path.exists(pathcat):
        print('file check unsuccessful, %s' % (pathcat))
        return
    pathsky = dirdata + 'sky'
    if conf.sky:
        pathsky = dirdata + conf.sky
    import libstarid
    ls = libstarid.libstarid()
    ls.write_sky(conf.pathsky, conf.pathcat)

def rstarpairs(conf):
    pathstarpairs = dirdata + 'starpairs'
    if conf.starpairs:
        pathstarpairs = dirdata + conf.starpairs
    if not os.path.exists(pathstarpairs):
        print('file check unsuccessful, %s' % (pathstarpairs))
        return
    import libstarid
    ls = libstarid.libstarid()
    ls.read_starpairs(conf.pathstarpairs)

def wstarpairs(conf):
    pathsky = dirdata + 'sky'
    if conf.sky:
        pathsky = dirdata + conf.sky
    if not os.path.exists(pathsky):
        print('file check unsuccessful, %s' % (pathsky))
        return
    pathstarpairs = dirdata + 'starpairs'
    if conf.starpairs:
        pathstarpairs = dirdata + conf.starpairs
    import libstarid
    ls = libstarid.libstarid()
    ls.write_starpairs(conf.pathstarpairs)

def test_sky(conf):
    import matplotlib.pyplot as plt
    import libstarid
    ls = libstarid.libstarid()
    ls.read_sky(conf.pathsky)
    imgdict = ls.image_generator(3)
    plt.matshow(-1 * imgdict['pixels'], cmap='Greys', interpolation='nearest')
    plt.show()

def write_sky(conf):
    import libstarid
    ls = libstarid.libstarid()
    ls.write_sky(conf.pathsky, conf.pathcat)

def startriangles(conf):
    import libstarid
    ls = libstarid.libstarid()
    ls.read_sky('../data/sky')
    ls.read_starpairs('../data/pairs')
    imgdict = ls.image_generator(conf.starndx)
    id = ls.startriangles(imgdict['pixels'])
    print(id)

if __name__ == "__main__":
    args = Config.read_args()
    args.pathlog = '../log'
    conf = Config(args)
    if conf.test: test_sky(conf)
    if conf.rsky: rsky(conf)
    if conf.wsky: wsky(conf)
    if conf.rstarpairs: rstarpairs(conf)
    if conf.wstarpairs: wstarpairs(conf)



