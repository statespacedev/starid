"""particle filters, aka sequential monte carlo sampling processors. sampling here is random, not deterministic as in
the sigma point kalman filter - and the idea of resampling and evolving newer, fitter particles comes to the fore.
the particles are generated randomly and new ones are introduced freely, but generally guided by evolving beliefs
around the model and external reality - things are at the least in the general regions of the 'genetic algorithms' -
survival-of-the-fittest is a core concept, but the 'genetic material' concept is de-emphasized in favor of using
sensors to bring in new additional data from the external world over time. everything here is about evolution of
beliefs over time, rather than static optimizations at a single point in time - sensor-based closed-loop feedback
estimation and control.

the particles live, die, and are born via survival-of-the-fittest 'resampling' - new particles essentially kill old
particles - things are quite red of tooth and claw. when new data comes in, some of the particles agree with the data
better than other particles - the former are fitter, the latter are less fit. 'resample' the less fit particles -
replace them with new random particles that are more fit - that agree with the data better - then life goes on until
the next new data comes in, the endless cycle repeats - evolve, update, evolve, update.

particles can soak up arbitrary amounts of hardware flops - just keep adding more particles. for a state vector with
x components, spkf gives 2x + 1 particles - two sigma point particles for each state component, and one particle
representing the estimated mean state. with sequential monte carlo, there's simply a 'cloud' of particles swarming
about over time - at any time, the cloud's estimated mean state can be estimated using kernel density methods,
likewise for the shape or scatter of the cloud and various measures of uncertainty."""
import numpy as np


class Particle:
    """particle filter, sequential monte carlo processor"""

    def __init__(self, conf):
        self.conf, self.log = conf, []
        self.run = self.run_model

    def run_model(self, model):
        """basic sampling-importance-resampling particle filter, bootstrap particle filter, condensation particle
        filter, survival of the fittest algorithm"""
        sim, _, h = model.entities()
        X, predict, update, resample = model.pf.entities()
        for t, o, u in sim():
            X = predict(X, u)
            W = update(X, o)
            x = np.sum(np.multiply(W, X), axis=1).reshape(-1, 1)
            X = resample(X, W)
            self.log.append([t, x, h(x)])


if __name__ == "__main__":
    pass
