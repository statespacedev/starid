import sys, os, logging, time, argparse

class Config():
    def __init__(self, args):
        self.cwd = os.getcwd()
        self.args = args
        self.pathlog = args.pathlog
        self.logger = self.start_logger(self.pathlog, args.dontlogstdout)
        self.logger.info('args, ' + str(vars(self.args)))

    @staticmethod
    def read_args():
        parser = argparse.ArgumentParser('starid')
        parser.add_argument('-t', '--test', help='show test image', action='store_true')
        parser.add_argument('--cat', help='filename for skymap text file', dest='cat', action='store', type=str)
        parser.add_argument('--sky', help='filename for sky binary file', dest='sky', action='store', type=str)
        parser.add_argument('--starpairs', help='filename for starpairs binary file', dest='pairs', action='store', type=str)
        parser.add_argument('--rsky', help='read sky binary file', action='store_true')
        parser.add_argument('--wsky', help='write sky binary file', action='store_true')
        parser.add_argument('--rstarpairs', help='read starpairs binary file', action='store_true')
        parser.add_argument('--wstarpairs', help='write starpairs binary file', action='store_true')
        args = parser.parse_args()
        return args

    @staticmethod
    def start_logger(pathlog, dontlogstdout=False):
        class StreamToLogger(object):
            def __init__(self, logger, log_level=logging.INFO):
                self.logger = logger
                self.log_level = log_level
                self.linebuf = ''
            def write(self, buf):
                for line in buf.rstrip().splitlines():
                    self.logger.log(self.log_level, line.rstrip())
            def flush(self):
                pass
        datefmt = "%Y-%m-%dT%H:%M:%SZ"
        formatter = logging.Formatter('%(asctime)s, %(levelname)s, %(module)s, %(funcName)s, %(message)s', datefmt)
        logging.Formatter.converter = time.gmtime
        fh = logging.FileHandler(filename=pathlog, mode='a')
        fh.setFormatter(formatter)
        logger = logging.getLogger('logger')
        logger.addHandler(fh)
        logger.handlers[0].setLevel(logging.INFO)
        if not dontlogstdout:
            sl = StreamToLogger(logger, logging.INFO)
            sys.stdout = sl
        logger.setLevel(logging.INFO)
        return logger

if __name__ == "__main__":
    pass
