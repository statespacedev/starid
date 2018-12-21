import sys, os, logging, time, argparse
sys.path.append('build/lib.linux-x86_64-3.6')
import libstarid

class Config():
    def __init__(self, args):
        self.cwd = os.getcwd() # expect */starid, not */starid/starid
        self.args = args

        self.dirsky = args.dirsky
        self.namecat = 'cat'
        self.namesky = 'sky'
        self.ls = libstarid.libstarid()
        if not os.path.exists(self.dirsky + self.namesky):
            self.ls.write_sky(self.dirsky + self.namesky, self.dirsky + self.namecat)
        self.ls.read_sky(self.dirsky + self.namesky)

        self.namesentences = 'lang-sentences'
        self.namevocab = 'lang-vocab'
        self.lang_sentences_per_target = 100

        self.pathlog = 'log'
        self.logger = self.start_logger(self.pathlog)
        self.logger.info('[cwd|args], %s, %s' % (self.cwd, str(vars(self.args))))

    @staticmethod
    def read_args():
        parser = argparse.ArgumentParser('starid')
        parser.add_argument('--dirsky', help='path to skymap directory', type=str, default='./data/')
        parser.add_argument('-t', '--test', help='show test star image', action='store_true')
        args = parser.parse_args()
        if not args.dirsky[-1] == '/': args.dirsky += '/'
        return args

    @staticmethod
    def start_logger(pathlog):
        datefmt = "%Y-%m-%dT%H:%M:%SZ"
        formatter = logging.Formatter('%(asctime)s, %(levelname)s, %(module)s, %(funcName)s, %(message)s', datefmt)
        logging.Formatter.converter = time.gmtime
        fh = logging.FileHandler(filename=pathlog, mode='a')
        fh.setFormatter(formatter)
        logger = logging.getLogger('logger')
        logger.addHandler(fh)
        logger.handlers[0].setLevel(logging.INFO)
        logger.setLevel(logging.INFO)
        return logger

if __name__ == "__main__":
    pass
