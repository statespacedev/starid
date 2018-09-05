import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import math, os
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

def diststr(x):
    return str(math.ceil(x / .1))

class Starimg:

    def __init__(self, starndx, info):
        self.starndx = starndx # starndx of target star
        self.info = info
        # use info to generate a 28 by 28 image pixel matrix
        self.image = np.zeros((28,28))
        for rowndx in range(len(self.info)):
            self.image[int(self.info[rowndx, 0]), int(self.info[rowndx, 1])] = 1.0
        # starlist is for info ready for writing nouns, verbs, and sentences
        self.starlist = []
        # self.starlist.append([int(self.starndx), 0., 0., 0.])
        # todo check pixels are unique within starlist
        for row in self.info:
            starndx = int(row[2])
            x = row[1] - 13.5
            y = 13.5 - row[0]
            r = math.ceil(math.sqrt(x**2 + y**2) * 100.) / 100.
            self.starlist.append([starndx, int(row[0]), int(row[1]), x, y, r])
        self.starlist = sorted(self.starlist, key=lambda x: x[5]) # sorted(self.starlist, key = lambda x: (x[3], x[0])
        if len(self.starlist) < 6: # too few stars
            self.lang = None
            return
        self.lang = Lang1(self.starlist, self.starndx)

    def plot_image(self):
        plt.matshow(-1 * self.image, cmap='Greys', interpolation='nearest')
        plt.show()

    def print_starlist(self):
        import pprint
        pprint.pprint(self.starlist)

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
            self.geom = 'v:' + diststr(d0) + ':' + diststr(d1) + ':' + diststr(d2)
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
            self.geom = 'n:' + diststr(sideab) + ':' + diststr(sidebc) + ':' + diststr(sideca)
            self.ids = 'n:' + stara + ':' + starb + ':' + starc

def generate_sentences_for_star(starndx, numsentences, verbose=False):
    sentences = {}
    for cnt in range(numsentences):
        outdict = ls.image_generator(starndx)
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

from nmt import model as nmt_model
from nmt import model_helper
from nmt import train
from nmt import inference
from nmt.utils import misc_utils as utils
from nmt.utils import vocab_utils
global hparams
dirdata = '../data/'

hparams = tf.contrib.training.HParams(
    src='geom',
    tgt='ids',
    dev_prefix=dirdata+'test1',
    test_prefix=dirdata+'test2',
    vocab_prefix=dirdata+'vocab',
    train_prefix=dirdata+'train',
    out_dir=dirdata+'nmt_model',
    num_units=128,
    num_layers=2,
    dropout=0.2,
    unit_type='lstm',
    encoder_type='uni',
    residual=False,
    num_residual_layers=0,
    time_major=True,
    num_embeddings_partitions=0,
    attention='',
    attention_architecture='standard',
    pass_hidden_state=True,
    optimizer='sgd',
    num_train_steps=100,
    batch_size=128,
    init_op='uniform',
    init_weight=0.1,
    max_gradient_norm=5.0,
    learning_rate=1.0,
    warmup_steps=0,
    warmup_scheme='t2t',
    start_decay_step=0,
    decay_factor=1.0,
    decay_steps=10000,
    learning_rate_decay_scheme='',
    colocate_gradients_with_ops=True,
    num_buckets=5,
    max_train=0,
    src_max_len=50,
    tgt_max_len=50,
    src_max_len_infer=False,
    tgt_max_len_infer=False,
    source_reverse=True,
    infer_batch_size=32,
    beam_width=0,
    length_penalty_weight=0.0,
    num_translations_per_input=1,
    sos='<s>',
    eos='</s>',
    subword_option='',
    check_special_token=True,
    share_vocab=False,
    forget_bias=1.0,
    num_gpus=1,
    epoch_step=0,
    steps_per_stats=100,
    steps_per_external_eval=None,
    metrics=['bleu'],
    log_device_placement=False,
    random_seed=None,
    override_loaded_hparams=False,
    use_char_encode=False,
    num_sampled_softmax=0,
    num_encoder_layers=4,
    num_decoder_layers=4,
    num_enc_emb_partitions=0,
    num_dec_emb_partitions=0,
    src_embed_file=None,
    tgt_embed_file=None,
    language_model=False,
    decay_scheme=None,
    num_keep_ckpts=0,
)

src_vocab_file = hparams.vocab_prefix + '.' + hparams.src
tgt_vocab_file = hparams.vocab_prefix + '.' + hparams.tgt

src_vocab_size, src_vocab_file = vocab_utils.check_vocab(
    src_vocab_file,
    hparams.out_dir,
    check_special_token=hparams.check_special_token,
    sos=hparams.sos,
    eos=hparams.eos,
    unk=vocab_utils.UNK)

tgt_vocab_size, tgt_vocab_file = vocab_utils.check_vocab(
    tgt_vocab_file,
    hparams.out_dir,
    check_special_token=hparams.check_special_token,
    sos=hparams.sos,
    eos=hparams.eos,
    unk=vocab_utils.UNK)

hparams.add_hparam("src_vocab_size", src_vocab_size)
hparams.add_hparam("tgt_vocab_size", tgt_vocab_size)
hparams.add_hparam("src_vocab_file", src_vocab_file)
hparams.add_hparam("tgt_vocab_file", tgt_vocab_file)

def train_minimalist():
    model_creator = nmt_model.Model
    train_model = model_helper.create_train_model(model_creator, hparams)
    train_sess = tf.Session(config=utils.get_config_proto(log_device_placement=hparams.log_device_placement), graph=train_model.graph)
    with train_model.graph.as_default():
        loaded_train_model, global_step = model_helper.create_or_load_model(train_model.model, hparams.out_dir, train_sess, 'train')
    summary_writer = tf.summary.FileWriter(os.path.join(hparams.out_dir, 'train_log'), train_model.graph)
    train_sess.run(train_model.iterator.initializer, feed_dict={train_model.skip_count_placeholder: (hparams.batch_size * hparams.epoch_step)})
    global_step = 0
    while global_step < hparams.num_train_steps:
        try:
            #(_, step_loss, step_predict_count, step_summary, global_step, step_word_count, batch_size) = loaded_train_model.train(train_sess)
            output = loaded_train_model.train(train_sess)
            global_step += 1
        except tf.errors.OutOfRangeError:
            utils.print_out('# epoch completed, step %d' % global_step)
            train_sess.run(train_model.iterator.initializer, feed_dict={train_model.skip_count_placeholder: 0})
            continue
    #summary_writer.add_summary(step_summary, global_step)
    loaded_train_model.saver.save(train_sess, os.path.join(hparams.out_dir, 'checkpoint.ckpt'), global_step=global_step)
    summary_writer.close()

def eval_minimalist():
    model_creator = nmt_model.Model
    train_model = model_helper.create_train_model(model_creator, hparams)
    eval_model = model_helper.create_eval_model(model_creator, hparams)
    train_sess = tf.Session(config=utils.get_config_proto(log_device_placement=hparams.log_device_placement), graph=train_model.graph)
    eval_sess = tf.Session(config=utils.get_config_proto(log_device_placement=hparams.log_device_placement), graph=eval_model.graph)
    with train_model.graph.as_default():
        loaded_train_model, global_step = model_helper.create_or_load_model(train_model.model, hparams.out_dir, train_sess, 'train')
    summary_writer = tf.summary.FileWriter(os.path.join(hparams.out_dir, 'train_log'), train_model.graph)
    train_sess.run(train_model.iterator.initializer, feed_dict={train_model.skip_count_placeholder: (hparams.batch_size * hparams.epoch_step)})
    train.run_internal_eval(eval_model, eval_sess, hparams.out_dir, hparams, summary_writer)
    global_step = 0
    while global_step < hparams.num_train_steps:
        try:
            (_, step_loss, step_predict_count, step_summary, global_step, step_word_count, batch_size) = loaded_train_model.train(train_sess)
            global_step += 1
        except tf.errors.OutOfRangeError:
            utils.print_out('# epoch completed, step %d' % global_step)
            train_sess.run(train_model.iterator.initializer, feed_dict={train_model.skip_count_placeholder: 0})
            continue
    train.run_internal_eval(eval_model, eval_sess, hparams.out_dir, hparams, summary_writer)
    summary_writer.add_summary(step_summary, global_step)
    loaded_train_model.saver.save(train_sess, os.path.join(hparams.out_dir, 'checkpoint.ckpt'), global_step=global_step)
    summary_writer.close()

def infer_minimalist():
    model_creator = nmt_model.Model
    infer_model = model_helper.create_infer_model(model_creator, hparams)
    dev_src_file = "%s.%s" % (hparams.dev_prefix, hparams.src)
    dev_tgt_file = "%s.%s" % (hparams.dev_prefix, hparams.tgt)
    sample_src_data = inference.load_data(dev_src_file)
    sample_tgt_data = inference.load_data(dev_tgt_file)
    infer_sess = tf.Session(config=utils.get_config_proto(log_device_placement=hparams.log_device_placement), graph=infer_model.graph)
    with infer_model.graph.as_default():
        loaded_infer_model, global_step = model_helper.create_or_load_model(infer_model.model, hparams.out_dir, infer_sess, 'infer')
    infer_sess.run(infer_model.iterator.initializer, feed_dict={infer_model.src_placeholder: sample_src_data, infer_model.batch_size_placeholder: hparams.infer_batch_size})
    summary_writer = tf.summary.FileWriter(os.path.join(hparams.out_dir, 'infer_log'), infer_model.graph)
    train.run_sample_decode(infer_model, infer_sess, hparams.out_dir, hparams, summary_writer, sample_src_data, sample_tgt_data)
    train.run_external_eval(infer_model, infer_sess, hparams.out_dir, hparams, summary_writer)
    summary_writer.close()

if __name__ == '__main__':
    if not tf.gfile.Exists(dirdata): tf.gfile.MakeDirs(dirdata)
    create_vocabulary_files(dirdata)
    create_sentence_files(path=dirdata, prefix='train', sentences_per_itr=100, numitrs=10)
    create_sentence_files(path=dirdata, prefix='test1', sentences_per_itr=100, numitrs=10)
    create_sentence_files(path=dirdata, prefix='test2', sentences_per_itr=100, numitrs=10)

    if not tf.gfile.Exists(hparams.out_dir): tf.gfile.MakeDirs(hparams.out_dir)
    #train_minimalist()
    # eval_minimalist()
    # infer_minimalist()
