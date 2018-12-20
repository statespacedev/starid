import math

class Sentence:
    def __init__(self, img):
        self.noun0g = 'n:na'
        self.noun0i = 'n:' + str(img.targetndx)
        self.noun1 = self.Noun(img.starlist[0:3])
        self.noun2 = self.Noun(img.starlist[3:6])
        self.verb1 = self.Verb(self.noun1)
        self.verb2 = self.Verb(self.noun1, self.noun2)
        self.geometry = self.noun1.geom + ' ' + self.verb1.geom + ' ' + self.noun0g + ' , ' + self.verb2.geom + ' ' + self.noun2.geom + ' .'
        self.identifiers = self.noun1.ids + ' ' + self.verb1.ids + ' ' + self.noun0i + ' , ' + self.verb2.ids + ' ' + self.noun2.ids + ' .'

    class Verb:
        def __init__(self, nouna, nounb=None):
            xa = [nouna.sides[0][5], nouna.sides[1][5], nouna.sides[2][5]]
            ya = [nouna.sides[0][6], nouna.sides[1][6], nouna.sides[2][6]]
            xb = [0., 0., 0.]
            yb = [0., 0., 0.]
            if nounb:
                xb = [nounb.sides[0][5], nounb.sides[1][5], nounb.sides[2][5]]
                yb = [nounb.sides[0][6], nounb.sides[1][6], nounb.sides[2][6]]
            d0 = math.sqrt((xa[0] - xb[0]) ** 2 + (ya[0] - yb[0]) ** 2)
            d1 = math.sqrt((xa[1] - xb[1]) ** 2 + (ya[1] - yb[1]) ** 2)
            d2 = math.sqrt((xa[2] - xb[2]) ** 2 + (ya[2] - yb[2]) ** 2)
            self.geom = 'v:' + str(math.ceil(d0/.1)) + ':' + str(math.ceil(d1/.1)) + ':' + str(math.ceil(d2/.1))
            self.ids = self.geom

    class Noun:
        def __init__(self, stars):
            self.stars = stars # star input for just this noun, three stars
            id = [self.stars[0][0], self.stars[1][0], self.stars[2][0]]
            x = [self.stars[0][3], self.stars[1][3], self.stars[2][3]]
            y = [self.stars[0][4], self.stars[1][4], self.stars[2][4]]
            side01 = math.sqrt((x[0] - x[1])**2 + (y[0] - y[1])**2)
            side12 = math.sqrt((x[1] - x[2])**2 + (y[1] - y[2])**2)
            side20 = math.sqrt((x[2] - x[0])**2 + (y[2] - y[0])**2)
            sides = [
                [0, 1, id[0], id[1], side01, x[0], y[0], x[1], y[1]],
                [1, 2, id[1], id[2], side12, x[1], y[1], x[2], y[2]],
                [2, 0, id[2], id[0], side20, x[2], y[2], x[0], y[0]]]
            sides = sorted(sides, key=lambda side: (side[4], side[0])) # increasing side length
            sideab = sides[0][4]
            sidebc = sides[1][4]
            sideca = sides[2][4]
            if (sides[0][0] == 0 and sides[1][0] == 1) \
                    or (sides[0][0] == 1 and sides[1][0] == 2) \
                    or (sides[0][0] == 2 and sides[1][0] == 0):
                stara = str(sides[0][2])
                starb = str(sides[0][3])
                starc = str(sides[1][3])
            else:
                stara = str(sides[0][3])
                starb = str(sides[0][2])
                starc = str(sides[1][2])
            self.sides = sides
            self.geom = 'n:' + str(math.ceil(sideab/.1)) + ':' + str(math.ceil(sidebc/.1)) + ':' + str(math.ceil(sideca/.1))
            self.ids = 'n:' + stara + ':' + starb + ':' + starc

class Vocab:
    def __init__(self, conf):
        self.conf = conf
        self.starndxs = []
        self.geom = {}
        self.ids = {}
        self.geom['<unk>'] = 1000
        self.geom['<s>'] = 1000
        self.geom['</s>'] = 1000
        self.ids['<unk>'] = 1000
        self.ids['<s>'] = 1000
        self.ids['</s>'] = 1000

    def add(self, sentences, targetndx):
        self.starndxs.append([targetndx, len(sentences)])  # number of unique sentences for this star
        for key, value in sentences.items():
            geom = value[1].split(' ')
            for word in geom:
                if word not in self.geom:
                    self.geom[word] = 1
                else:
                    self.geom[word] += 1
            ids = value[2].split(' ')
            for word in ids:
                if word not in self.ids:
                    self.ids[word] = 1
                else:
                    self.ids[word] += 1

    def write(self):
        mode = 'wt'
        with open(self.conf.dirsky + self.conf.namevocab + '.geom', mode) as geom, open(self.conf.dirsky + self.conf.namevocab + '.labels', 'w') as labels:
            for key in self.geom.keys():
                geom.write('%s\n' % key)
            for key in self.ids.keys():
                labels.write('%s\n' % key)

class Sentences():
    def __init__(self, conf):
        self.conf = conf

    def batch(self, batchndx):
        from starimage import Starimg
        self.sentences = {}
        for sentndx in range(self.conf.lang_batchsize):
            img = Starimg(conf, targetndx)
            sentence = Sentence(img)
            keytxt = sentence.geometry + ' : ' + sentence.identifiers
            if keytxt not in self.sentences:
                self.sentences[keytxt] = [1, sentence.geometry, sentence.identifiers]
            else:
                self.sentences[keytxt][0] += 1
        mode = 'at'
        if batchndx == 0: mode = 'wt'
        with open(conf.dirsky + conf.namesentences + '.geom', mode) as geom, open(conf.dirsky + conf.namesentences + '.labels', mode) as labels:
            for key, value in self.sentences.items():
                geom.write('%s\n' % value[1])
                labels.write('%s\n' % value[2])

if __name__ == '__main__':
    from config import Config
    args = Config.read_args()
    conf = Config(args)
    vocab = Vocab(conf)
    s = Sentences(conf)
    for batchndx in range(conf.lang_batches):
        for targetndx in range(11): # starndx 4 has less than six stars, so include starndx 10
            s.batch(batchndx)
            vocab.add(s.sentences, targetndx)
    vocab.write()
    pass


