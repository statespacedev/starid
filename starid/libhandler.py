import sys, os
sys.path.append('build/cmake/libstarid')
import libstarid

class LibHandler():
    """todo"""
    def __init__(self):
        # self.cwd = os.getcwd() # expect */starid, not */starid/starid
        # self.args = args
        self.dirsky = './data/'
        self.namecat = 'cat'
        self.namesky = 'sky'

        self.api = libstarid.Libstarid()
        if not os.path.exists(self.dirsky + self.namesky):
            self.api.write_sky(self.dirsky + self.namesky, self.dirsky + self.namecat)
        self.api.read_sky(self.dirsky + self.namesky)
