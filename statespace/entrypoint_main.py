"""the place to configure runtime env."""
import sys
import argparse


def main():
    """configure and call process_model(). -h and --help are built in, so 'python statespace -h' displays an options
    text. """
    cli = argparse.ArgumentParser()
    cli.add_argument('-p', '--processor', choices=['ekf', 'spkf', 'pf'], default='ekf', type=str)
    cli.add_argument('-m', '--model', choices=['one', 'three', 'bearings'], default='three', type=str)
    cli.add_argument('-f', '--factorized', dest='factorized', action='store_true', default=False)
    conf = cli.parse_args()
    if not len(sys.argv) > 1:
        conf.processor = 'ekf'
        conf.model = 'one'
        conf.factorized = False
    process_model(conf)
    return


def process_model(conf):
    """bring in a processor and a model, process the model, evaluate the results. """
    processor, model = None, None
    match conf.processor:
        case 'ekf':
            from statespace.processors.extended_kalman_filter import Kalman
            processor = Kalman(conf)
        case 'spkf':
            from statespace.processors.sigma_point_kalman_filter import SigmaPoint
            processor = SigmaPoint(conf)
        case 'pf':
            from statespace.processors.particle_filter import Particle
            processor = Particle(conf)
    match conf.model:
        case 'one':
            from statespace.models.one_state import Onestate
            model = Onestate(conf)
        case 'three':
            from statespace.models.three_state import Threestate
            model = Threestate(conf)
        case 'bearings':
            from statespace.models.bearings_only import BearingsOnly
            model = BearingsOnly(conf)
    processor.run(model)
    model.eval.estimate(processor.log)
    # model.eval.autocorr.run(processor.log)
    model.eval.show()


if __name__ == "__main__":
    main()
