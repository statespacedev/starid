import tensorflow as tf
import os
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
    if not tf.gfile.Exists(hparams.out_dir): tf.gfile.MakeDirs(hparams.out_dir)
    train_minimalist()
    # eval_minimalist()
    # infer_minimalist()

