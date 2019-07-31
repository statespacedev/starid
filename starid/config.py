import sys, os, logging, time, argparse
sys.path.append('build/lib.linux-x86_64-3.6')
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

        self.namesentences = 'lang-sentences'
        self.namevocab = 'lang-vocab'
        self.lang_targets = 10
        self.lang_train_sentences = 30000
        self.lang_train_batch = 64
        self.lang_epochs = 1
        self.lang_dirckpt = './data/lang-ckpt/'

        # self.pathlog = 'log'
        # self.logger = self.start_logger(self.pathlog)
        # self.logger.info('[cwd|args], %s, %s' % (self.cwd, str(vars(self.args))))

    @staticmethod
    def read_args():
        parser = argparse.ArgumentParser('starid')
        parser.add_argument('-d', '--demo', help='demo star image', action='store_true')
        args = parser.parse_args()
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
