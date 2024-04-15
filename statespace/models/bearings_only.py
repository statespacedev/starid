"""the bearings only problem has some interesting history. it's basically about being on a sub. your sub is
travelling along steadily and you begin hearing the sound of a ship at some bearing. over time and as the bearing
changes, you can estimate the relative position and velocity of the ship. at some point you make a course change for
your sub to pursue the ship. """
import math
import numpy as np
import matplotlib.pyplot as plt
from statespace.models.base_model import BaseModel, BaseEKF, BaseSPKF, BasePF, BaseEval, Autocorr, Log
from scipy.stats import norm
# noinspection PyProtectedMember
from filterpy.monte_carlo import systematic_resample


class BearingsOnly(BaseModel):
    """bearings-only tracking problem"""

    def __init__(self, conf):
        super().__init__(conf)
        self.tsteps = 100  # 2hr = 7200sec / 100
        self.dt = .02  # hrs, .02 hr = 72 sec
        self.xsim = np.array([[0., 15., 20., -10.]]).T
        self.varprocsim = 1e-6
        self.varobssim = 6e-4
        self.Rsim = self.varobssim
        self.Qsim = self.varprocsim * np.eye(2)
        self.Gsim = np.zeros([4, 2])
        self.Gsim[0, 0] = self.dt ** 2 / 2
        self.Gsim[1, 1] = self.dt ** 2 / 2
        self.Gsim[2, 0] = self.dt
        self.Gsim[3, 1] = self.dt
        self.ekf = EKF(self)
        self.spkf = SPKF(self)
        self.pf = PF(self)
        self.eval = Eval(self)

    def simulation(self):
        """time series of states x, inputs u, and obs y. """
        for tstep in range(1, self.tsteps):
            t = tstep * self.dt
            u = np.array([[0., 0., 0., 0.]]).T
            if t == 0.5: u[2], u[3] = -24., 10.
            self.xsim = self.f(self.xsim, 'sim') + u
            ysim = self.h(self.xsim, 'sim')
            self.log.append([t, self.xsim, ysim])
            yield t, ysim, u

    def f(self, x, *args):
        """state evolution equation. """
        x = np.array([[x[0, 0] + self.dt * x[2, 0], x[1, 0] + self.dt * x[3, 0], x[2, 0], x[3, 0]]]).T
        if 'sim' in args:
            w = self.Gsim @ np.multiply(np.random.randn(2), math.sqrt(self.varprocsim))
            return x + w
        return x

    def h(self, x, *args):
        """observation equation. """
        y = np.arctan2(x[0, 0], x[1, 0])
        if 'sim' in args:
            v = np.random.randn() * math.sqrt(self.varobssim)
            return y + v
        return y


class EKF(BaseEKF):
    """what's common across kalman filter models. """

    def __init__(self, parent):
        super().__init__()
        self.parent = parent
        self.x0 = np.array([[0., 20., 20., -10.]]).T
        self.P0 = 1 * np.eye(4)
        self.varproc = 1e-6
        self.varobs = 6e-4
        self.R = self.varobs
        self.Q = self.varproc * np.eye(2)
        self.G = np.zeros([4, 2])
        self.G[0, 0] = self.parent.dt ** 2 / 2
        self.G[1, 1] = self.parent.dt ** 2 / 2
        self.G[2, 0] = self.parent.dt
        self.G[3, 1] = self.parent.dt

    def F(self, x):
        """state evolution matrix. """
        F = np.eye(4)
        F[0, 2] = self.parent.dt
        F[1, 3] = self.parent.dt
        return F

    def H(self, x):
        """observation sensitivity matrix. """
        dsqr = x[0, 0] ** 2 + x[1, 0] ** 2
        return np.array([[x[1, 0] / dsqr, -x[0, 0] / dsqr, 0, 0]])


class SPKF(BaseSPKF):
    """what's common across sigma point kalman filter models. """

    def __init__(self, parent):
        super().__init__()
        self.parent = parent
        n, k = 4, 1
        w1, w2 = k / (n + k), .5 / (n + k)
        self.W = np.array([[w1, w2, w2, w2, w2, w2, w2, w2, w2]])
        self.WM = np.tile(self.W, (self.parent.xsim.shape[0], 1))

    def XY(self, x, P, u):
        """sigma points update. """
        n, k = 4, 1
        col1 = x
        col2 = x + np.sqrt((n + k) * np.array([[P[0, 0], 0, 0, 0]]).T)
        col3 = x + np.sqrt((n + k) * np.array([[0, P[1, 1], 0, 0]]).T)
        col4 = x + np.sqrt((n + k) * np.array([[0, 0, P[2, 2], 0]]).T)
        col5 = x + np.sqrt((n + k) * np.array([[0, 0, 0, P[3, 3]]]).T)
        col6 = x - np.sqrt((n + k) * np.array([[P[0, 0], 0, 0, 0]]).T)
        col7 = x - np.sqrt((n + k) * np.array([[0, P[1, 1], 0, 0]]).T)
        col8 = x - np.sqrt((n + k) * np.array([[0, 0, P[2, 2], 0]]).T)
        col9 = x - np.sqrt((n + k) * np.array([[0, 0, 0, P[3, 3]]]).T)
        X = np.column_stack((col1, col2, col3, col4, col5, col6, col7, col8, col9))
        for i in range(9): X[:, i] = (self.parent.f(X[:, i].reshape(-1, 1)) + u).flatten()
        Y = np.zeros((1, 9))
        for i in range(9): Y[0, i] = self.parent.h(X[:, i].reshape(-1, 1)).flatten()
        return X, Y


class PF(BasePF):
    """what's common across particle filter models. """

    def __init__(self, parent):
        super().__init__()
        self.parent = parent
        self.n = 250
        self.x0 = np.array([[0., 20., 20., -10.]]).T
        self.Q = self.parent.ekf.G @ self.parent.ekf.Q @ self.parent.ekf.G.T

    def X0(self):
        """initial particles. """
        return self.x0 + np.multiply(np.random.randn(self.x0.shape[0], self.n), np.diag(np.sqrt(self.Q)).reshape(-1, 1))

    def predict(self, X, u):
        """evolution forward in time of particles. """
        def f(x):
            w = np.multiply(np.random.randn(1, x.shape[0]), np.sqrt(np.diag(self.Q))).T
            base = np.array([[x[0] + self.parent.dt * x[2], x[1] + self.parent.dt * x[3], x[2], x[3]]]).T
            return base + w + u

        for i in range(X.shape[1]): X[:, i] = f(X[:, i]).flatten()
        return X

    def update(self, X, o):
        """observational update of particles. """
        dsqr = X[0, :] ** 2 + X[1, :] ** 2
        W = norm.pdf((X[1, :] - X[0, :]) / dsqr, o, np.sqrt(self.parent.R)).reshape(1, -1)
        return W / np.sum(W)

    def resample(self, x, W):
        """resampling of particles. """
        ndxs = systematic_resample(W.T)
        return x[:, ndxs]


class Eval(BaseEval):
    """evaluating processor results. """

    def __init__(self, parent):
        super().__init__()
        self.parent = parent
        self.autocorr = Autocorr(parent)

    def model(self):
        """plot true timeseries. """
        lw, logm = 1, Log(self.parent.log)
        plt.figure()
        plt.subplot(3, 2, 1), plt.plot(logm.t, logm.x[:, 0], linewidth=lw), plt.ylabel('x[0]')
        plt.subplot(3, 2, 2), plt.plot(logm.t, logm.x[:, 1], linewidth=lw), plt.ylabel('x[1]')
        plt.subplot(3, 2, 3), plt.plot(logm.t, logm.x[:, 2], linewidth=lw), plt.ylabel('x[2]')
        plt.subplot(3, 2, 4), plt.plot(logm.t, logm.x[:, 3], linewidth=lw), plt.ylabel('x[3]')
        plt.subplot(3, 2, 5), plt.plot(logm.t, logm.y, linewidth=lw), plt.ylabel('y')

    def estimate(self, proclog):
        """plot estimated results. """
        lw, logm, logp = 1, Log(self.parent.log), Log(proclog)
        plt.figure()
        plt.subplot(3, 2, 1)
        plt.plot(logm.t, logm.x[:, 0], 'b', linewidth=lw), plt.ylabel('x0')
        plt.plot(logp.t, logp.x[:, 0], 'r--', linewidth=lw)
        plt.subplot(3, 2, 2)
        plt.plot(logm.t, logm.x[:, 1], 'b', linewidth=lw), plt.ylabel('x1')
        plt.plot(logp.t, logp.x[:, 1], 'r--', linewidth=lw)
        plt.subplot(3, 2, 3)
        plt.plot(logm.t, logm.x[:, 2], 'b', linewidth=lw), plt.ylabel('x2')
        plt.plot(logp.t, logp.x[:, 2], 'r--', linewidth=lw)
        plt.subplot(3, 2, 4)
        plt.plot(logm.t, logm.x[:, 3], 'b', linewidth=lw), plt.ylabel('x3')
        plt.plot(logp.t, logp.x[:, 3], 'r--', linewidth=lw)
        plt.subplot(3, 2, 5)
        plt.plot(logm.t, logm.y, 'b', linewidth=lw), plt.ylabel('y')
        plt.plot(logp.t, logp.y, 'r--', linewidth=lw)
        plt.subplot(3, 2, 6), plt.plot(logp.t, logm.y - logp.y, 'b', linewidth=lw), plt.ylabel('y err')


if __name__ == "__main__":
    pass
