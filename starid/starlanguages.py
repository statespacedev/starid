import math, os
import numpy as np
import tensorflow as tf
import matplotlib as plt
from starimage import Starimg
tf.enable_eager_execution()

class Sentences():
    def __init__(self, conf):
        self.conf = conf

    def write(self):
        from random import randint
        with open(conf.dirsky + conf.namesentences, 'wt') as fout:
            cnt = 0
            while cnt < self.conf.lang_sentences:
                target = randint(0, conf.lang_targets - 1)
                img = Starimg(conf, target)
                if len(img.starlist) < 6: continue
                sentence = Sentences.Sentence(img)
                fout.write('%s\t%s\n' % (sentence.geometry, sentence.labels))
                cnt += 1
                if cnt % 100 == 0: print(cnt)

    class Sentence:
        def __init__(self, img):
            self.noun0g = 'n|na'
            self.noun0i = 'n|' + str(img.targetndx)
            self.noun1 = Sentences.Sentence.Noun(img.starlist[0:3])
            self.noun2 = Sentences.Sentence.Noun(img.starlist[3:6])
            self.verb1 = Sentences.Sentence.Verb(self.noun1)
            self.verb2 = Sentences.Sentence.Verb(self.noun1, self.noun2)
            self.geometry = self.noun1.geom + ' ' + self.verb1.geom + ' ' + self.noun0g + ', ' + self.verb2.geom + ' ' + self.noun2.geom + '.'
            self.labels = self.noun1.ids + ' ' + self.verb1.ids + ' ' + self.noun0i + ', ' + self.verb2.ids + ' ' + self.noun2.ids + '.'

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

class Data():
    def __init__(self, conf):
        self.conf = conf
        pairs = self.create_dataset(conf.dirsky + conf.namesentences, num_examples=self.conf.lang_sentences)
        self.inp_lang = Data.LanguageIndex(l2 for l1, l2 in pairs)
        self.targ_lang = Data.LanguageIndex(l1 for l1, l2 in pairs)
        self.input_tensor = [[self.inp_lang.word2idx[s] for s in l2.split(' ')] for l1, l2 in pairs]
        self.target_tensor = [[self.targ_lang.word2idx[s] for s in l1.split(' ')] for l1, l2 in pairs]
        self.max_length_inp, self.max_length_tar = self.max_length(self.input_tensor), self.max_length(self.target_tensor)
        self.input_tensor = tf.keras.preprocessing.sequence.pad_sequences(self.input_tensor, maxlen=self.max_length_inp, padding='post')
        self.target_tensor = tf.keras.preprocessing.sequence.pad_sequences(self.target_tensor, maxlen=self.max_length_tar, padding='post')
        self.buffer_size = len(self.input_tensor)
        self.n_batch = self.buffer_size//self.conf.lang_batch_size
        self.vocab_inp_size = len(self.inp_lang.word2idx)
        self.vocab_tar_size = len(self.targ_lang.word2idx)
        self.dataset = tf.data.Dataset.from_tensor_slices((self.input_tensor, self.target_tensor)).shuffle(self.buffer_size)
        self.dataset = self.dataset.batch(conf.lang_batch_size, drop_remainder=True)

    def preprocess_sentence(self, w):
        import re
        w = w.lower().strip()
        w = re.sub(r"([?.!,¿])", r" \1 ", w)
        w = re.sub(r'[" "]+', " ", w)
        w = re.sub(r"[^a-zA-Z0-9?.!,¿|]+", " ", w)
        w = w.rstrip().strip()
        w = '<start> ' + w + ' <end>'
        return w

    def create_dataset(self, path, num_examples):
        lines = open(path, encoding='UTF-8').read().strip().split('\n')
        word_pairs = [[self.preprocess_sentence(w) for w in l.split('\t')] for l in lines[:num_examples]]
        return word_pairs

    def max_length(self, tensor):
        return max(len(t) for t in tensor)

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

class Model():
    def __init__(self, conf, vocab_inp_size, vocab_tar_size):
        self.conf = conf
        embedding_dim = 256
        units = 1024
        self.encoder = Model.Encoder(vocab_inp_size, embedding_dim, units, conf.lang_batch_size)
        self.decoder = Model.Decoder(vocab_tar_size, embedding_dim, units, conf.lang_batch_size)
        self.optimizer = tf.train.AdamOptimizer()
        self.checkpoint_dir = self.conf.lang_dirckpt
        self.checkpoint_prefix = os.path.join(self.checkpoint_dir, "ckpt")
        self.checkpoint = tf.train.Checkpoint(optimizer=self.optimizer, encoder=self.encoder, decoder=self.decoder)

    def loss_function(self, real, pred):
        mask = 1 - np.equal(real, 0)
        loss_ = tf.nn.sparse_softmax_cross_entropy_with_logits(labels=real, logits=pred) * mask
        return tf.reduce_mean(loss_)

    def gru(self, units):
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

    def training(self, data):
        import time
        for epoch in range(self.conf.lang_epochs):
            start = time.time()
            hidden = self.encoder.initialize_hidden_state()
            total_loss = 0
            for (batch, (inp, targ)) in enumerate(data.dataset):
                loss = 0
                with tf.GradientTape() as tape:
                    enc_output, enc_hidden = self.encoder(inp, hidden)
                    dec_hidden = enc_hidden
                    dec_input = tf.expand_dims([data.targ_lang.word2idx['<start>']] * self.conf.lang_batch_size, 1)
                    for t in range(1, targ.shape[1]):
                        predictions, dec_hidden, _ = self.decoder(dec_input, dec_hidden, enc_output)
                        loss += self.loss_function(targ[:, t], predictions)
                        dec_input = tf.expand_dims(targ[:, t], 1)
                batch_loss = (loss / int(targ.shape[1]))
                total_loss += batch_loss
                variables = self.encoder.variables + self.decoder.variables
                gradients = tape.gradient(loss, variables)
                self.optimizer.apply_gradients(zip(gradients, variables))
                if batch % 100 == 0:
                    print('Epoch {} Batch {} Loss {:.4f}'.format(epoch + 1, batch, batch_loss.numpy()))
            if (epoch + 1) % 1 == 0:
                self.checkpoint.save(file_prefix = self.checkpoint_prefix)
            print('Epoch {} Loss {:.4f}'.format(epoch + 1, total_loss / data.n_batch))
            print('Time taken for 1 epoch {} sec\n'.format(time.time() - start))

    def evaluate(self, sentence, encoder, decoder, inp_lang, targ_lang, max_length_inp, max_length_targ):
        attention_plot = np.zeros((max_length_targ, max_length_inp))
        sentence = self.preprocess_sentence(sentence)
        inputs = [inp_lang.word2idx[i] for i in sentence.split(' ')]
        inputs = tf.keras.preprocessing.sequence.pad_sequences([inputs], maxlen=max_length_inp, padding='post')
        inputs = tf.convert_to_tensor(inputs)
        result = ''
        hidden = [tf.zeros((1, units))]
        enc_out, enc_hidden = encoder(inputs, hidden)
        dec_hidden = enc_hidden
        dec_input = tf.expand_dims([targ_lang.word2idx['<start>']], 0)
        for t in range(max_length_targ):
            predictions, dec_hidden, attention_weights = decoder(dec_input, dec_hidden, enc_out)
            attention_weights = tf.reshape(attention_weights, (-1, ))
            attention_plot[t] = attention_weights.numpy()
            predicted_id = tf.argmax(predictions[0]).numpy()
            result += targ_lang.idx2word[predicted_id] + ' '
            if targ_lang.idx2word[predicted_id] == '<end>':
                return result, sentence, attention_plot
            dec_input = tf.expand_dims([predicted_id], 0)
        return result, sentence, attention_plot

    def plot_attention(self, attention, sentence, predicted_sentence):
        fig = plt.figure(figsize=(10,10))
        ax = fig.add_subplot(1, 1, 1)
        ax.matshow(attention, cmap='viridis')
        fontdict = {'fontsize': 14}
        ax.set_xticklabels([''] + sentence, fontdict=fontdict, rotation=90)
        ax.set_yticklabels([''] + predicted_sentence, fontdict=fontdict)
        plt.show()

    def restore(self):
        self.checkpoint.restore(tf.train.latest_checkpoint(self.checkpoint_dir))

    def translate(self, sentence, encoder, decoder, inp_lang, targ_lang, max_length_inp, max_length_targ):
        result, sentence, attention_plot = self.evaluate(sentence, encoder, decoder, inp_lang, targ_lang, max_length_inp, max_length_targ)
        print('Input: {}'.format(sentence))
        print('Predicted translation: {}'.format(result))
        # attention_plot = attention_plot[:len(result.split(' ')), :len(sentence.split(' '))]
        # plot_attention(attention_plot, sentence.split(' '), result.split(' '))

if __name__ == '__main__':
    mode = 2
    from config import Config
    args = Config.read_args()
    conf = Config(args)
    if mode == 0:
        sentences = Sentences(conf)
        sentences.write()
    elif mode == 1:
        data = Data(conf)
        model = Model(conf, len(data.inp_lang.word2idx), len(data.targ_lang.word2idx))
        model.training(data)
    elif mode == 2:
        data = Data(conf)
        model = Model(conf, len(data.inp_lang.word2idx), len(data.targ_lang.word2idx))
        model.restore()

    pass


