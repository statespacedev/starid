
def demo(conf, targetndx):
    from starimage import Starimg
    img = Starimg(conf, targetndx)
    img.print_starlist()
    img.plot_image()

if __name__ == "__main__":
    from config import Config
    args = Config.read_args()
    conf = Config(args)
    if conf.args.demo: demo(conf, targetndx=3)
