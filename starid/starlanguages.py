import math, os, re
import numpy as np
import tensorflow as tf
from starimage import Starimg
tf.enable_eager_execution()

class Language():
    def __init__(self, conf):
        self.conf = conf

    def sentences_write(self):
        from random import randint
        with open(self.conf.dirsky + self.conf.namesentences, 'wt') as fout:
            cnt = 0
            while cnt < self.conf.lang_sentences:
                target = randint(0, self.conf.lang_targets - 1)
                img = Starimg(self.conf, target)
                if len(img.starlist) < 6: continue
                sentence = Language.Sentence(img)
                fout.write('%s\t%s\n' % (sentence.labels, sentence.geometry))
                cnt += 1
                if cnt % 100 == 0: print(cnt)

    def sentences_gen(self, numtargets, numsentences=None):
        from random import randint
        cnt = 0
        while True:
            target = randint(0, numtargets - 1)
            img = Starimg(self.conf, target)
            if len(img.starlist) < 6: continue
            cnt += 1
            if numsentences and cnt > numsentences: return
            sentences = Language.Sentence(img)
            yield [sentences.labels, sentences.geometry]

    @staticmethod
    def vocabulary(mode=None):
        if not mode: # dynamic
            sentence_pairs = []
            for s in Language.sentences_gen(self.conf.lang_targets, numsentences=50000):
                sentence_pairs.append(s)
            inp_lang = Language.LanguageIndex(mode='lang', lang=(l2 for l1, l2 in sentence_pairs))
            targ_lang = Language.LanguageIndex(mode='lang', lang=(l1 for l1, l2 in sentence_pairs))
            vocab_cnt_inp, vocab_cnt__targ = len(inp_lang.vocab), len(targ_lang.vocab)
            input_tensor = [[inp_lang.word2idx[s] for s in l2.split(' ')] for l1, l2 in sentence_pairs]
            target_tensor = [[targ_lang.word2idx[s] for s in l1.split(' ')] for l1, l2 in sentence_pairs]
            max_length_inp = max(len(t) for t in input_tensor)
            max_length_tar = max(len(t) for t in target_tensor)
            return inp_lang, targ_lang, max_length_inp, max_length_tar
        elif mode == 'full_inp':
            return map(str, range(35))
        elif mode == 'full_targ':
            return map(str, range(8876))

    class Sentence:
        def __init__(self, img):
            self.noun0g = ''
            self.noun0i = str(img.targetndx)
            self.noun1 = Language.Sentence.Noun(img.starlist[0:3])
            self.noun2 = Language.Sentence.Noun(img.starlist[3:6])
            self.verb1 = Language.Sentence.Verb(self.noun1)
            self.verb2 = Language.Sentence.Verb(self.noun1, self.noun2)
            self.geometry = '<start> ' + self.noun1.geom + ' ' + self.verb1.geom + ' , ' + self.verb2.geom + ' ' + self.noun2.geom + ' . <end>'
            self.labels = '<start> ' + self.noun1.ids + ' ' + self.verb1.ids + ' ' + self.noun0i + ' , ' + self.verb2.ids + ' ' + self.noun2.ids + ' . <end>'

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
                self.geom = str(round(d0)) + ' ' + str(round(d1)) + ' ' + str(round(d2))
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
                self.geom = str(round(sideab)) + ' ' + str(round(sidebc)) + ' ' + str(round(sideca))
                self.ids = stara + ' ' + starb + ' ' + starc

    class LanguageIndex():
        def __init__(self, mode, lang=None):
            self.word2idx = {}
            self.idx2word = {}
            self.vocab = {'<start>', '<end>', '.', ','}
            if mode == 'dynamic':
                for phrase in lang: self.vocab.update(phrase.split(' '))
            elif mode == 'full_inp':
                self.vocab.update(Language.vocabulary(mode='full_inp'))
            elif mode == 'full_targ':
                self.vocab.update(Language.vocabulary(mode="full_targ"))
            self.vocab = sorted(self.vocab)
            self.word2idx['<pad>'] = 0
            for index, word in enumerate(self.vocab): self.word2idx[word] = index + 1
            for word, index in self.word2idx.items(): self.idx2word[index] = word

class Dataset():
    def __init__(self, conf, pathin=None):
        self.conf = conf
        lang = Language(conf)
        if pathin:
            lines = open(pathin).read().strip().split('\n')
            sentence_pairs = [[s for s in l.split('\t')] for l in lines[:self.conf.lang_train_sentences]]
            self.inp_lang = lang.LanguageIndex(mode='dynamic', lang=(l2 for l1, l2 in sentence_pairs))
            self.targ_lang = lang.LanguageIndex(mode='dynamic', lang=(l1 for l1, l2 in sentence_pairs))
        else:
            self.inp_lang = lang.LanguageIndex(mode='full_inp')
            self.targ_lang = lang.LanguageIndex(mode='full_targ')
            sentence_pairs = []
            for s in lang.sentences_gen(self.conf.lang_targets, self.conf.lang_train_sentences):
                sentence_pairs.append(s)
        self.input_tensor = [[self.inp_lang.word2idx[s] for s in l2.split(' ')] for l1, l2 in sentence_pairs]
        self.target_tensor = [[self.targ_lang.word2idx[s] for s in l1.split(' ')] for l1, l2 in sentence_pairs]
        self.max_length_inp = max(len(t) for t in self.input_tensor)
        self.max_length_tar = max(len(t) for t in self.target_tensor)
        self.input_tensor = tf.keras.preprocessing.sequence.pad_sequences(self.input_tensor, maxlen=self.max_length_inp, padding='post')
        self.target_tensor = tf.keras.preprocessing.sequence.pad_sequences(self.target_tensor, maxlen=self.max_length_tar, padding='post')
        self.buffer_size = len(self.input_tensor)
        self.n_batch = self.buffer_size//self.conf.lang_train_batch
        self.dataset = tf.data.Dataset.from_tensor_slices((self.input_tensor, self.target_tensor)).shuffle(self.buffer_size)
        self.dataset = self.dataset.batch(conf.lang_train_batch, drop_remainder=True)

class Model():
    def __init__(self, conf, data):
        self.conf = conf
        self.data = data
        self.embedding_dim = 256
        self.units = 1024
        self.encoder = Model.Encoder(len(data.inp_lang.word2idx), self.embedding_dim, self.units, conf.lang_train_batch)
        self.decoder = Model.Decoder(len(data.targ_lang.word2idx), self.embedding_dim, self.units, conf.lang_train_batch)
        self.optimizer = tf.train.AdamOptimizer()
        self.checkpoint_dir = self.conf.lang_dirckpt
        self.checkpoint_prefix = os.path.join(self.checkpoint_dir, "ckpt")
        self.checkpoint = tf.train.Checkpoint(optimizer=self.optimizer, encoder=self.encoder, decoder=self.decoder)

    @staticmethod
    def loss_function(real, pred):
        mask = 1 - np.equal(real, 0)
        loss_ = tf.nn.sparse_softmax_cross_entropy_with_logits(labels=real, logits=pred) * mask
        return tf.reduce_mean(loss_)

    @staticmethod
    def gru(units):
        if tf.test.is_gpu_available():
            return tf.keras.layers.CuDNNGRU(units, return_sequences=True, return_state=True,
                                            recurrent_initializer='glorot_uniform')
        else:
            return tf.keras.layers.GRU(units, return_sequences=True, return_state=True,
                                       recurrent_activation='sigmoid', recurrent_initializer='glorot_uniform')

    class Encoder(tf.keras.Model):
        def __init__(self, vocab_size, embedding_dim, enc_units, batch_sz):
            super(Model.Encoder, self).__init__()
            self.batch_sz = batch_sz
            self.enc_units = enc_units
            self.embedding = tf.keras.layers.Embedding(vocab_size, embedding_dim)
            self.gru = Model.gru(self.enc_units)

        def call(self, x, hidden):
            x = self.embedding(x)
            output, state = self.gru(x, initial_state = hidden)
            return output, state

        def initialize_hidden_state(self):
            return tf.zeros((self.batch_sz, self.enc_units))

    class Decoder(tf.keras.Model):
        def __init__(self, vocab_size, embedding_dim, dec_units, batch_sz):
            super(Model.Decoder, self).__init__()
            self.batch_sz = batch_sz
            self.dec_units = dec_units
            self.embedding = tf.keras.layers.Embedding(vocab_size, embedding_dim)
            self.gru = Model.gru(self.dec_units)
            self.fc = tf.keras.layers.Dense(vocab_size)
            self.W1 = tf.keras.layers.Dense(self.dec_units)
            self.W2 = tf.keras.layers.Dense(self.dec_units)
            self.V = tf.keras.layers.Dense(1)

        def call(self, x, hidden, enc_output):
            hidden_with_time_axis = tf.expand_dims(hidden, 1)
            score = self.V(tf.nn.tanh(self.W1(enc_output) + self.W2(hidden_with_time_axis)))
            attention_weights = tf.nn.softmax(score, axis=1)
            context_vector = attention_weights * enc_output
            context_vector = tf.reduce_sum(context_vector, axis=1)
            x = self.embedding(x)
            x = tf.concat([tf.expand_dims(context_vector, 1), x], axis=-1)
            output, state = self.gru(x)
            output = tf.reshape(output, (-1, output.shape[2]))
            x = self.fc(output)
            return x, state, attention_weights

        def initialize_hidden_state(self):
            return tf.zeros((self.batch_sz, self.dec_units))

    def training(self):
        import time
        for epoch in range(self.conf.lang_epochs):
            start = time.time()
            hidden = self.encoder.initialize_hidden_state()
            total_loss = 0
            for (batch, (inp, targ)) in enumerate(self.data.dataset):
                loss = 0
                with tf.GradientTape() as tape:
                    enc_output, enc_hidden = self.encoder(inp, hidden)
                    dec_hidden = enc_hidden
                    dec_input = tf.expand_dims([self.data.targ_lang.word2idx['<start>']] * self.conf.lang_train_batch, 1)
                    for t in range(1, targ.shape[1]):
                        predictions, dec_hidden, _ = self.decoder(dec_input, dec_hidden, enc_output)
                        loss += self.loss_function(targ[:, t], predictions)
                        dec_input = tf.expand_dims(targ[:, t], 1)
                batch_loss = (loss / int(targ.shape[1]))
                total_loss += batch_loss
                variables = self.encoder.variables + self.decoder.variables
                gradients = tape.gradient(loss, variables)
                self.optimizer.apply_gradients(zip(gradients, variables))
                if (batch + 1) % 10 == 0:
                    print('epoch {} batch {} loss {:.4f}'.format(epoch + 1, batch + 1, batch_loss.numpy()))
            if (epoch + 1) % 1 == 0:
                self.checkpoint.save(file_prefix = self.checkpoint_prefix)
            print('epoch {} loss {:.4f}'.format(epoch + 1, total_loss / self.data.n_batch))
            print('time taken for 1 epoch {} sec\n'.format(time.time() - start))

    def evaluate(self, sentence):
        attention_plot = np.zeros((self.data.max_length_tar, self.data.max_length_inp))
        # sentence = Data.preprocess_sentence(sentence)
        inputs = [self.data.inp_lang.word2idx[i] for i in sentence.split(' ')]
        inputs = tf.keras.preprocessing.sequence.pad_sequences([inputs], maxlen=self.data.max_length_inp, padding='post')
        inputs = tf.convert_to_tensor(inputs)
        result = ''
        hidden = [tf.zeros((1, self.units))]
        enc_out, enc_hidden = self.encoder(inputs, hidden)
        dec_hidden = enc_hidden
        dec_input = tf.expand_dims([self.data.targ_lang.word2idx['<start>']], 0)
        for t in range(self.data.max_length_tar):
            predictions, dec_hidden, attention_weights = self.decoder(dec_input, dec_hidden, enc_out)
            attention_weights = tf.reshape(attention_weights, (-1, ))
            attention_plot[t] = attention_weights.numpy()
            predicted_id = tf.argmax(predictions[0]).numpy()
            result += self.data.targ_lang.idx2word[predicted_id] + ' '
            if self.data.targ_lang.idx2word[predicted_id] == '<end>':
                return result, sentence, attention_plot
            dec_input = tf.expand_dims([predicted_id], 0)
        return result, sentence, attention_plot

    @staticmethod
    def plot_attention(attention, sentence, predicted_sentence):
        import matplotlib.pyplot as plt
        fig = plt.figure()
        ax = fig.add_subplot(1, 1, 1)
        ax.matshow(attention, cmap='viridis')
        fontdict = {'fontsize': 10}
        ax.set_xticks(np.arange(.5, len(sentence), 1))
        ax.set_yticks(np.arange(.5, len(predicted_sentence), 1))
        ax.set_xticklabels(sentence, fontdict=fontdict, rotation=90, ha='right')
        ax.set_yticklabels(['<start>'] + predicted_sentence, fontdict=fontdict, va='bottom')
        plt.show()

    def restore(self):
        self.checkpoint.restore(tf.train.latest_checkpoint(self.checkpoint_dir))

    def translate(self, sentence):
        result, sentence, attention_plot = self.evaluate(sentence)
        print('input: {}'.format(sentence))
        print('translation: {}'.format(result))
        attention_plot = attention_plot[:len(result.split(' ')), :len(sentence.split(' '))]
        Model.plot_attention(attention_plot, sentence.split(' '), result.split(' '))

def main():
    mode = 11
    from config import Config
    conf = Config()
    if mode == 0:
        sen = Language(conf)
        sen.sentences_write()
    elif mode == 1:
        sen = Language(conf)
        sen.vocabulary()
    elif mode == 10:
        pathin = './data/lang-sentences'
        data = Dataset(conf, pathin)
        model = Model(conf, data)
        model.training()
    elif mode == 11:
        pathin = './data/lang-sentences'
        data = Dataset(conf, pathin)
        model = Model(conf, data)
        model.restore()
        model.translate('<start> 9 15 17 6 10 8 , 14 15 18 2 17 18 . <end>')
    elif mode == 20:
        data = Dataset(conf)
        model = Model(conf, data)
        model.training()
    elif mode == 21:
        data = Dataset(conf)
        model = Model(conf, data)
        model.restore()
        model.translate('<start> 2 7 7 4 3 5 , 9 4 8 4 7 10 . <end>')

# <start> 8854 64 8859 6 10 8 3 , 14 15 18 8815 8808 10 . <end>	<start> 9 15 17 6 10 8 , 14 15 18 2 17 18 . <end>

if __name__ == '__main__':
    main()

    pass


