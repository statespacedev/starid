"""a simple as possible one-state example with non linear temporal and observation updates. it's a common example in
the candy and jazwinisky books. based on real world reentry vehicle tracking. """
import math
import numpy as np
import matplotlib.pyplot as plt
from statespace.models.base_model import BaseModel, BaseEKF, BaseSPKF, BasePF, BaseEval, Autocorr, Log
from scipy.stats import norm
# noinspection PyProtectedMember
from filterpy.monte_carlo import systematic_resample


class Onestate(BaseModel):
    """one-state reference model"""

    def __init__(self, conf):
        super().__init__(conf)
        self.tsteps = 151
        self.dt = .01
        self.xsim = np.array([[2.]]).T
        self.varprocsim = 1e-6 * np.array([[1]]).T
        self.varobssim = 6e-4
        self.Rsim = self.varobssim
        self.ekf = EKF(self)
        self.spkf = SPKF(self)
        self.pf = PF(self)
        self.eval = Eval(self)

    def simulation(self):
        """time series of states x, inputs u, and obs y. """
        for tstep in range(1, self.tsteps):
            t = tstep * self.dt
            u = np.array([0]).T
            self.xsim = self.f(self.xsim, 'sim') + u
            ysim = self.h(self.xsim, 'sim')
            self.log.append([t, self.xsim, ysim])
            yield t, ysim, u

    def f(self, x, *args):
        """state evolution equation. """
        x = (1 - .05 * self.dt) * x + (.04 * self.dt) * x ** 2
        if 'sim' in args:
            w = math.sqrt(self.varprocsim) * np.random.randn()
            return x + w
        return x

    def h(self, x, *args):
        """observation equation. """
        y = x[0, 0] ** 2 + x[0, 0] ** 3
        if 'sim' in args:
            v = math.sqrt(self.Rsim) * np.random.randn()
            return y + v
        return y


class EKF(BaseEKF):
    """what's common across kalman filter models. """

    def __init__(self, parent):
        super().__init__()
        self.parent = parent
        self.x0 = np.array([[2.1]]).T
        self.P0 = .1 * np.eye(1)
        self.varproc = 1e-6 * np.array([[1]]).T
        self.varobs = 6e-4
        self.R = self.varobs
        self.Q = self.varproc * np.eye(1)
        self.G = np.eye(1)

    def F(self, x):
        """state evolution matrix. """
        A = np.eye(1)
        A[0, 0] = 1 - .05 * self.parent.dt + .08 * self.parent.dt * x
        return A

    def H(self, x):
        """observation sensitivity matrix. """
        return np.array([[2 * x[0, 0] + 3 * x[0, 0] ** 2]])


class SPKF(BaseSPKF):
    """what's common across sigma point kalman filter models. """

    def __init__(self, parent):
        super().__init__()
        self.parent = parent
        n, k = 1, 1
        w1, w2 = k / (n + k), .5 / (n + k)
        self.W = np.array([[w1, w2, w2]])
        self.WM = np.tile(self.W, (self.parent.xsim.shape[0], 1))
        self.kappa = 1
        self.k0 = 1 + self.kappa
        self.nlroot = math.sqrt(self.k0)
        self.S = np.linalg.cholesky(parent.ekf.P0)
        self.Sproc = np.linalg.cholesky(parent.ekf.Q)
        self.Sobs = np.linalg.cholesky(np.diag(parent.ekf.R * np.array([1])))
        self.Xtil = np.zeros((1, 3))
        self.Ytil = np.zeros((1, 3))
        self.Pxy = np.zeros((1, 1))

    def XY(self, x, P, u):
        """sigma points update. """
        k = 1
        col1 = x
        col2 = x + np.sqrt(k * np.array([[P[0, 0]]]).T)
        col3 = x - np.sqrt(k * np.array([[P[0, 0]]]).T)
        X = np.column_stack((col1, col2, col3))
        for i in range(3): X[:, i] = (self.parent.f(X[:, i].reshape(-1, 1)) + u).flatten()
        Y = np.zeros((1, 3))
        for i in range(3): Y[0, i] = self.parent.h(X[:, i].reshape(-1, 1)).flatten()
        return X, Y

    def XYcho(self, x, S, u):
        """cholesky sigma points update. """
        col1 = x
        col2 = x + self.nlroot * S[:, 0].reshape(-1, 1)
        col3 = x - self.nlroot * S[:, 0].reshape(-1, 1)
        X = np.column_stack((col1, col2, col3))
        for i in range(3): X[:, i] = (self.parent.f(X[:, i].reshape(-1, 1)) + u).flatten()
        Y = np.zeros((1, 3))
        for i in range(3): Y[0, i] = self.parent.h(X[:, i].reshape(-1, 1)).flatten()
        return X, Y


class PF(BasePF):
    """what's common across particle filter models. """

    def __init__(self, parent):
        super().__init__()
        self.parent = parent
        self.n = 250
        self.x0 = np.array([[2.05]]).T

    def X0(self):
        """initial particles. """
        return self.x0 + np.multiply(np.random.randn(self.x0.shape[0], self.n),
                                     np.diag(np.sqrt(self.parent.Q)).reshape(-1, 1))

    def predict(self, X, u):
        """evolution forward in time of particles. """
        return (1 - .05 * self.parent.dt) * X + (.04 * self.parent.dt) * X ** 2 + math.sqrt(
            self.parent.varproc) * np.random.randn(self.n)

    def update(self, X, o):
        """observational update of particles. """
        W = norm.pdf(X ** 2 + X ** 3, o, np.sqrt(self.parent.R))
        return W / np.sum(W)

    def resample(self, x, W):
        """resampling of particles. """
        return x[:, systematic_resample(W.T)]


class Eval(BaseEval):
    """evaluating processor results. """

    def __init__(self, parent):
        super().__init__()
        self.parent = parent
        self.autocorr = Autocorr(parent)

    def estimate(self, proclog):
        """plot estimated results. """
        lw, logm, logp = 1, Log(self.parent.log), Log(proclog)
        plt.figure()
        plt.subplot(2, 2, 1)
        plt.plot(logm.t, logm.x[:, 0], 'b', linewidth=lw), plt.ylabel('x[0]')
        plt.plot(logp.t, logp.x[:, 0], 'r--', linewidth=lw)
        plt.subplot(2, 2, 2)
        plt.plot(logm.t, logm.y, 'b', linewidth=lw), plt.ylabel('y')
        plt.plot(logm.t, logp.y, 'r--', linewidth=lw)
        plt.subplot(2, 2, 3), plt.plot(logp.t, logm.y - logp.y, 'b', linewidth=lw), plt.ylabel('y err')


if __name__ == "__main__":
    pass
