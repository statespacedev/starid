"""placeholder for what could grow to become a higher-level statespace model - with individual models inheriting and
overriding. for now it's mostly acting as a skelton to sketch out what such a beast could potentially look like -
what entities and activities make sense here?

if a particular model doesn't need something sketched out here, don't override it. if something's needed,
override it.
"""
import numpy as np
import matplotlib.pyplot as plt


class BaseModel:
    """what's common for the model across various processors? in short, a simulation of states and observations over
    some time period - a simulation time series. the sim is driven by vector valued functions - generally non-linear
    - a state evolution equation f, and an observation equation h."""

    def __init__(self, conf):
        self.conf = conf
        self.log = []
        self.ekf = BaseEKF()
        self.spkf = BaseSPKF()
        self.pf = BasePF()

    def entities(self):
        """entities needed for a processor. """
        return self.simulation, self.f, self.h

    def simulation(self):
        """time series of states x, inputs u, and obs y. """
        pass

    def f(self, x):
        """state evolution equation. """
        return None

    def h(self, x):
        """observation equation. """
        return None


class BaseEKF:
    """what's common across kalman filter models. """

    def __init__(self):
        self.R = None
        self.Q = None
        self.G = None
        self.x0 = None
        self.P0 = None

    def entities(self):
        """entities needed for an extended kalman filter processor. """
        return self.F, self.H, self.R, self.Q, self.G, self.x0, self.P0

    def F(self, x):
        """state evolution matrix. """
        return None

    def H(self, x):
        """observation sensitivity matrix. """
        return None


# noinspection PyUnusedLocal
class BaseSPKF:
    """what's common across sigma point kalman filter models. """

    def __init__(self):
        self.W = None
        self.WM = None
        self.Xtil = None
        self.Ytil = None
        self.Pxy = None
        self.S = None
        self.Sproc = None
        self.Sobs = None

    def entities(self):
        """entities needed for a sigma point kalman filter processor. """
        return self.XY, self.W, self.WM

    def entities_cholesky(self):
        """entities needed for cholesky factorization sigma point kalman filter processor. """
        return self.XYcho, self.W, self.Xtil, self.Ytil, self.Pxy, self.S, self.Sproc, self.Sobs

    def XY(self, x, P, u):
        """sigma points update. """
        return None

    def XYcho(self, X, S, u):
        """cholesky sigma points update. """
        return None


class BasePF:
    """what's common across particle filter models. """

    def __init__(self):
        pass

    def entities(self):
        """entities needed for a particle filter processor. """
        return self.X0(), self.predict, self.update, self.resample

    def X0(self):
        """initial particles. """
        pass

    def predict(self, X, u):
        """evolution forward in time of particles. """
        return None

    def update(self, X, o):
        """observational update of particles. """
        return None

    def resample(self, X, W):
        """resampling of particles. """
        return None


class BaseEval:
    """evaluating processor results. """

    def __init__(self):
        self.autocorr = Autocorr()  # override this in child's constructor with self.autocorr = Autocorr(parent)

    def model(self):
        """plot true timeseries. """
        return None

    def estimate(self, proclog):
        """plot estimated results. """
        return None

    @staticmethod
    def show():
        """show the plots. """
        plt.show()


class Log:
    """logging of the results timeseries. """

    def __init__(self, log):
        self.t = np.array([x[0] for x in log])
        self.x = np.array([x[1] for x in log])
        self.y = np.array([x[2] for x in log])


class Autocorr:
    """auto correlation of results with themselves. """

    def __init__(self, parent=None):
        self.parent = parent

    def run(self, proclog):
        """run autocorrelation. """
        logp = Log(proclog)
        logm = Log(self.parent.log)
        self.tts = logm.t
        self.ets = logm.y - logp.y
        self.mhate = np.mean(self.ets)
        self.Rhatee = np.mean(np.power(self.ets, 2))
        n = len(self.ets)
        self.tau = 1.96 * np.sqrt(self.Rhatee / n)
        self.iszeromean = True
        if abs(self.mhate) > self.tau: self.iszeromean = False
        self.zmw = self.autocorr1(self.ets)
        self.plot()

    def plot(self):
        """show the plot. """
        plt.figure()
        lw = 1
        plt.plot(self.tts, self.zmw, linewidth=lw), plt.ylabel('autocorr')

    def autocorr1(self, x):
        """calculation version one. """
        n = x.size
        ac = np.zeros([n, 1])
        for k in range(1, n):  # k = 1 to n-1
            ac[k] = 0
            for t in range(1, n - k):  # t = 1 to n-k
                ac[k] += (self.ets[t - 1] - self.mhate) * (self.ets[t - 1 + k] - self.mhate)  # / (n-k)
        return ac

    @staticmethod
    def autocorr2(x):
        """calculation version two. """
        ac = np.correlate(x, x, mode='full')
        return ac[ac.size // 2:]


if __name__ == "__main__":
    pass
