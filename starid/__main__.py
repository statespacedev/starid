import sys, os, shutil
print('cwd, %s' % os.getcwd()) # expect */starid, not */starid/starid
if not os.path.exists('libstarid.cpython-36m-x86_64-linux-gnu.so'):
    shutil.copy2('build/lib.linux-x86_64-3.6/libstarid.cpython-36m-x86_64-linux-gnu.so', '.')
sys.path.append('.')

def test(conf, targetndx):
    from starimage import Starimg
    img = Starimg(conf, targetndx)
    img.print_starlist()
    img.plot_image()
    pass

if __name__ == "__main__":
    from config import Config
    args = Config.read_args()
    conf = Config(args)
    if conf.args.test: test(conf, targetndx=3)
