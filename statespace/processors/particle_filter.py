"""particle filters, aka sequential monte carlo sampling processors."""
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
