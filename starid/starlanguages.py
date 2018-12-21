import math
import tensorflow as tf

class Sentence:
    def __init__(self, img):
        self.noun0g = 'n|na'
        self.noun0i = 'n|' + str(img.targetndx)
        self.noun1 = self.Noun(img.starlist[0:3])
        self.noun2 = self.Noun(img.starlist[3:6])
        self.verb1 = self.Verb(self.noun1)
        self.verb2 = self.Verb(self.noun1, self.noun2)
        self.geometry = self.noun1.geom + ' ' + self.verb1.geom + ' ' + self.noun0g + ', ' + self.verb2.geom + ' ' + self.noun2.geom + '.'
        self.identifiers = self.noun1.ids + ' ' + self.verb1.ids + ' ' + self.noun0i + ', ' + self.verb2.ids + ' ' + self.noun2.ids + '.'

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
            self.geom = 'v|' + str(math.ceil(d0/.1)) + '|' + str(math.ceil(d1/.1)) + '|' + str(math.ceil(d2/.1))
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
            self.geom = 'n|' + str(math.ceil(sideab/.1)) + '|' + str(math.ceil(sidebc/.1)) + '|' + str(math.ceil(sideca/.1))
            self.ids = 'n|' + stara + '|' + starb + '|' + starc

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
        with open(conf.dirsky + conf.namesentences + '.geom', mode) as geom, \
                open(conf.dirsky + conf.namesentences + '.labels', mode) as labels, \
                open(conf.dirsky + conf.namesentences + '.paired', mode) as paired:
            for key, value in self.sentences.items():
                geom.write('%s\n' % value[1])
                labels.write('%s\n' % value[2])
                paired.write('%s\t%s\n' % (value[1], value[2]))

class Prep():
    def __init__(self, conf):
        self.conf = conf
        self.input_tensor, self.target_tensor, self.inp_lang, self.targ_lang, self.max_length_inp, self.max_length_targ = Prep.load_dataset(conf.dirsky + conf.namesentences + '.paired', num_examples=30)

    @staticmethod
    def preprocess_sentence(w):
        import re
        w = w.lower().strip()
        w = re.sub(r"([?.!,¿])", r" \1 ", w)
        w = re.sub(r'[" "]+', " ", w)
        w = re.sub(r"[^a-zA-Z0-9?.!,¿|]+", " ", w)
        w = w.rstrip().strip()
        w = '<start> ' + w + ' <end>'
        return w

    @staticmethod
    def create_dataset(path, num_examples):
        lines = open(path, encoding='UTF-8').read().strip().split('\n')
        word_pairs = [[Prep.preprocess_sentence(w) for w in l.split('\t')]  for l in lines[:num_examples]]
        return word_pairs

    class LanguageIndex():
        def __init__(self, lang):
            self.lang = lang
            self.word2idx = {}
            self.idx2word = {}
            self.vocab = set()
            self.create_index()

        def create_index(self):
            for phrase in self.lang: self.vocab.update(phrase.split(' '))
            self.vocab = sorted(self.vocab)
            self.word2idx['<pad>'] = 0
            for index, word in enumerate(self.vocab): self.word2idx[word] = index + 1
            for word, index in self.word2idx.items(): self.idx2word[index] = word

    @staticmethod
    def max_length(tensor):
        return max(len(t) for t in tensor)

    @staticmethod
    def load_dataset(path, num_examples):
        pairs = Prep.create_dataset(path, num_examples)
        inp_lang = Prep.LanguageIndex(l2 for l1, l2 in pairs)
        targ_lang = Prep.LanguageIndex(l1 for l1, l2 in pairs)
        input_tensor = [[inp_lang.word2idx[s] for s in l2.split(' ')] for l1, l2 in pairs]
        target_tensor = [[targ_lang.word2idx[s] for s in l1.split(' ')] for l1, l2 in pairs]
        max_length_inp, max_length_tar = Prep.max_length(input_tensor), Prep.max_length(target_tensor)
        input_tensor = tf.keras.preprocessing.sequence.pad_sequences(input_tensor, maxlen=max_length_inp, padding='post')
        target_tensor = tf.keras.preprocessing.sequence.pad_sequences(target_tensor, maxlen=max_length_tar, padding='post')
        return input_tensor, target_tensor, inp_lang, targ_lang, max_length_inp, max_length_tar

if __name__ == '__main__':

    mode = 0

    if mode == 0:
        from config import Config
        args = Config.read_args()
        conf = Config(args)
        s = Sentences(conf)
        for batchndx in range(conf.lang_batches):
            for targetndx in range(11): # starndx 4 has less than six stars, so include starndx 10
                s.batch(batchndx)
        prep = Prep(conf)
        pass

    elif mode == 1:
        from sklearn.model_selection import train_test_split
        input_tensor_train, input_tensor_val, target_tensor_train, target_tensor_val = train_test_split(input_tensor, target_tensor, test_size=0.2)
        len(input_tensor_train), len(target_tensor_train), len(input_tensor_val), len(target_tensor_val)
        BUFFER_SIZE = len(input_tensor_train)
        BATCH_SIZE = 64
        N_BATCH = BUFFER_SIZE//BATCH_SIZE
        embedding_dim = 256
        units = 1024
        vocab_inp_size = len(inp_lang.word2idx)
        vocab_tar_size = len(targ_lang.word2idx)
        dataset = tf.data.Dataset.from_tensor_slices((input_tensor_train, target_tensor_train)).shuffle(BUFFER_SIZE)
        dataset = dataset.batch(BATCH_SIZE, drop_remainder=True)

    pass


