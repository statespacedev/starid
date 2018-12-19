import tensorflow as tf
import math
import libstarid
ls = libstarid.libstarid()
ls.read_sky('../data/sky')
dirdata = '../data/'

class Vocabulary:

    def __init__(self):
        self.starndxs = []
        self.geom = {}
        self.ids = {}
        self.geom['<unk>'] = 1000
        self.geom['<s>'] = 1000
        self.geom['</s>'] = 1000
        self.ids['<unk>'] = 1000
        self.ids['<s>'] = 1000
        self.ids['</s>'] = 1000

    def update(self, sentences, starndx):
        self.starndxs.append([starndx, len(sentences)])  # number of unique sentences for this star
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

    def write_files(self, path):
        with open(path + 'vocab.geom', 'w') as fout:
            for key in self.geom.keys():
                fout.write('%s\n' % key)
        with open(path + 'vocab.ids', 'w') as fout:
            for key in self.ids.keys():
                fout.write('%s\n' % key)

class Lang1:

    def __init__(self, starlist, starndx):
        self.noun0g = 'n:na'
        self.noun0i = 'n:' + str(starndx)
        self.noun1 = self.Noun(starlist[0:3])
        self.noun2 = self.Noun(starlist[3:6])
        self.verb1 = self.Verb(self.noun1)
        self.verb2 = self.Verb(self.noun1, self.noun2)
        self.sentence_geom = self.noun1.geom + ' ' + self.verb1.geom + ' ' + self.noun0g + ' , ' \
                             + self.verb2.geom + ' ' + self.noun2.geom + ' .'
        self.sentence_ids = self.noun1.ids + ' ' + self.verb1.ids + ' ' + self.noun0i + ' , ' \
                            + self.verb2.ids + ' ' + self.noun2.ids + ' .'

    @staticmethod
    def diststr(x):
        return str(math.ceil(x / .1))

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
            self.geom = 'v:' + Lang1.diststr(d0) + ':' + Lang1.diststr(d1) + ':' + Lang1.diststr(d2)
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
            self.geom = 'n:' + Lang1.diststr(sideab) + ':' + Lang1.diststr(sidebc) + ':' + Lang1.diststr(sideca)
            self.ids = 'n:' + stara + ':' + starb + ':' + starc

def generate_sentences_for_star(starndx, numsentences, verbose=False):
    sentences = {}
    for cnt in range(numsentences):
        outdict = ls.image_generator(starndx)
        from starimage import Starimg
        starimg = Starimg(starndx=starndx, info=outdict['info'])
        if not starimg.lang: # too few stars
            continue
        keytxt = starimg.lang.sentence_geom + ' : ' + starimg.lang.sentence_ids
        if keytxt not in sentences:
            sentences[keytxt] = [1, starimg.lang.sentence_geom, starimg.lang.sentence_ids]
        else:
            sentences[keytxt][0] += 1
    if verbose:
        print(sorted(sentences.values(), key=lambda x: -x[0]))
    return sentences

def create_vocabulary_files(path):
    vocabulary = Vocabulary()
    for starndx in range(11): # starndx 4 has less than six stars, so include starndx 10
        sentences = generate_sentences_for_star(starndx=starndx, numsentences=1000)
        vocabulary.update(sentences=sentences, starndx=starndx)
    print(vocabulary.starndxs) # sentences per starndx
    vocabulary.write_files(path=dirdata)

def create_sentence_files(path, prefix, sentences_per_itr, numitrs):
    fgeom = open(path + prefix + '.geom', 'w')
    fids = open(path + prefix + '.ids', 'w')
    for itr in range(numitrs):
        for starndx in range(11): # starndx 4 has less than six stars, so include starndx 10
            sentences = generate_sentences_for_star(starndx=starndx, numsentences=sentences_per_itr)
            for key, value in sentences.items():
                fgeom.write('%s\n' % value[1])
                fids.write('%s\n' % value[2])
    fgeom.close()
    fids.close()

if __name__ == '__main__':
    if not tf.gfile.Exists(dirdata): tf.gfile.MakeDirs(dirdata)
    create_vocabulary_files(dirdata)
    create_sentence_files(path=dirdata, prefix='train', sentences_per_itr=100, numitrs=10)
    create_sentence_files(path=dirdata, prefix='test1', sentences_per_itr=100, numitrs=10)
    create_sentence_files(path=dirdata, prefix='test2', sentences_per_itr=100, numitrs=10)

