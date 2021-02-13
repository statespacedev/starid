import sys
sys.path.append('starid')

def test_001():
    from demo import Config, Starimg
    conf = Config()
    img = Starimg(conf, targetndx=3)
    assert(len(img.starlist) > 1)
