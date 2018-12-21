import sys
sys.path.append('build/lib.linux-x86_64-3.6')

def test(conf, targetndx):
    from starimage import Starimg
    img = Starimg(conf, targetndx)
    img.print_starlist()
    img.plot_image()
    from starlanguages import Sentence
    sent = Sentence(img)
    pass

if __name__ == "__main__":
    from config import Config
    args = Config.read_args()
    conf = Config(args)
    if conf.args.test: test(conf, targetndx=3)
