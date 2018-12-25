import sys
sys.path.append('starid')

from config import Config
conf = Config()

def test_001():
    from starimage import Starimg
    img = Starimg(conf, targetndx=3)
    assert(len(img.starlist) == 15)
