import math
import numpy as np
import matplotlib.pyplot as plt

class DecisionsEns():
    def __init__(self):
        self.decisionfunctionssig = []
        self.decisionfunctionsnoise = []
        self.sigdfvs = []
        self.sigpmfs = []
        self.noisedfvs = []
        self.noisepmfs = []
        self.pdets = []
        self.pfas = []

    def finalize(self):
        self.globalmin = np.floor(np.min(self.decisionfunctionsnoise))
        self.globalmax = np.ceil(np.max(self.decisionfunctionssig))
        self.decfuncx = np.linspace(self.globalmin, self.globalmax, self.globalmax - self.globalmin)
        for ndx in range(len(self.decisionfunctionssig)):
            sigdfv, sigpmf = self.decisionfunctionpmf(self.decisionfunctionssig[ndx])
            self.sigdfvs.append(sigdfv)
            self.sigpmfs.append(sigpmf)
            noisedfv, noisepmf = self.decisionfunctionpmf(self.decisionfunctionsnoise[ndx])
            self.noisedfvs.append(noisedfv)
            self.noisepmfs.append(noisepmf)
            pdet = np.zeros(len(sigpmf))
            pfa = np.zeros(len(noisepmf))
            lastndx = len(sigpmf) - 1
            for ndx in range(lastndx + 1):
                if ndx == 0:
                    pdet[ndx] = sigpmf[lastndx]
                    pfa[ndx] = noisepmf[lastndx]
                else:
                    pdet[ndx] = pdet[ndx - 1] + sigpmf[lastndx - ndx]
                    pfa[ndx] = pfa[ndx - 1] + noisepmf[lastndx - ndx]
            self.pdets.append(pdet)
            self.pfas.append(pfa)
        pass

    def decisionfunctionpmf(self, vals):
        pass
        # from pymc3.plots.kdeplot import fast_kde
        # vals = np.append(vals, [self.globalmin, self.globalmax])
        # pmf, min, max = fast_kde(vals, bw=4.5)
        # x = np.linspace(min, max, len(pmf))
        # pmf = pmf / sum(pmf)
        # return x, pmf

    def plot_roc(self):
        plt.figure()
        for ndx in range(len(self.pdets)):
            plt.plot(self.pfas[ndx], self.pdets[ndx], 'g', alpha=.1)
        plt.xlim(0., 1.)
        plt.ylim(0., 1.)
        plt.xlabel('probability false-alarm')
        plt.ylabel('probability detection')

    def plot_decisionfunctions(self):
        plt.figure()
        for ndx in range(len(self.decisionfunctionssig)):
            plt.plot(self.sigdfvs[ndx], self.sigpmfs[ndx], 'g', alpha=.1)
            plt.plot(self.noisedfvs[ndx], self.noisepmfs[ndx], 'b', alpha=.1)
        sigx, sigpmf = self.decisionfunctionpmf(self.decisionfunctionssig)
        noisex, noisepmf = self.decisionfunctionpmf(self.decisionfunctionsnoise)
        plt.plot(sigx, sigpmf, 'g')
        plt.plot(noisex, noisepmf, 'b')
        plt.xlabel('decision function output-value')
        plt.ylabel('p')

    def addsig(self, decisionfunction):
        self.decisionfunctionssig.append(decisionfunction)

    def addnoise(self, decisionfunction):
        self.decisionfunctionsnoise.append(decisionfunction)

class Decisions():
    def __init__(self, mode, tracker):
        self.mode = mode
        tracker.innovs.finalize()
        self.t = tracker.innovs.log[:, 0]
        self.yhat = tracker.innovs.log[:, 2]
        self.inn = tracker.innovs.log[:, 4]
        self.Rhatee = tracker.innovs.log[:, 5]
        self.decisionfunction = self.evaldecisionfunction()

    def evaldecisionfunction(self):
        decisionfunction = np.zeros(len(self.inn))
        for ndx in range(len(decisionfunction)):
            delta = (self.yhat[ndx]**2 / 8) - (self.inn[ndx]**2 / (2 * self.Rhatee[ndx]))
            if ndx == 0: decisionfunction[0] = delta
            else: decisionfunction[ndx] = decisionfunction[ndx-1] + delta
        return decisionfunction

class Dists():
    def __init__(self):
        self.log = []

    def update(self, t, tru, est):
        pass
        # from pymc3.plots.kdeplot import fast_kde
        # trupmf, trumin, trumax = fast_kde(tru)
        # estpmf, estmin, estmax = fast_kde(est)
        # truval = np.linspace(trumin, trumax, len(trupmf))
        # estval = np.linspace(estmin, estmax, len(estpmf))
        # trupmf = trupmf / sum(trupmf)
        # estpmf = estpmf / sum(estpmf)
        # self.log.append([t, truval, trupmf, estval, estpmf])

    def kld1(self, x1, x2):
        def kli(a, b): return np.sum(np.multiply(a, np.log(a)) - np.multiply(a, np.log(b)), axis=0)
        x1 = 1.0 * x1 / np.sum(x1, axis=0)
        x2 = 1.0 * x2 / np.sum(x2, axis=0)
        return (kli(x1, x2) + kli(x2, x1)) / 2

    def kld2(self, x1, x2):
        from scipy.stats import entropy
        return entropy(x1, x2)

    def plot(self, tlim = 100):
        import matplotlib.pyplot as plt
        est = []
        for rec in self.log: est.append([rec[0], rec[1][np.argmax(rec[2])], 0., rec[3][np.argmax(rec[4])], 0.])
        est = np.asarray(est)
        plt.figure()
        ax = plt.axes(projection='3d')
        ax.plot3D(est[:tlim,1], est[:tlim,0], est[:tlim,2], c='r', linewidth=1)
        ax.plot3D(est[:tlim,3], est[:tlim,0], est[:tlim,4], c='r', linewidth=1)
        for rec in self.log[:tlim]:
            ax.plot3D(rec[1], rec[0] * np.ones(len(rec[1])), rec[2], c='g', linewidth=1)
            ax.plot3D(rec[3], rec[0] * np.ones(len(rec[3])), rec[4], c='b', linewidth=1)
        plt.title('kl-divergence, %.1E' % (self.kld1(est[:tlim, 1], est[:tlim, 3])))
        ax.set_xlabel('x')
        ax.set_ylabel('t')
        ax.set_zlabel('p')
        plt.show()

class DistsEns():
    def __init__(self):
        self.log = []

    def update(self, distslog):
        for rec in distslog:
            truval, trupmf = rec[1], rec[2]
            maptru = truval[np.argmax(trupmf)]
            estval, estpmf = rec[3], rec[4]
            mapest = estval[np.argmax(estpmf)]
            self.log.append([maptru, mapest])

    def plot(self):
        pass
        # log = np.asarray(self.log)
        # from pymc3.plots.kdeplot import fast_kde
        # import matplotlib.pyplot as plt
        # maptrupmf, maptrumin, maptrumax = fast_kde(log[:, 0])
        # mapestpmf, mapestmin, mapestmax = fast_kde(log[:, 1])
        # maptruval = np.linspace(maptrumin, maptrumax, len(maptrupmf))
        # mapestval = np.linspace(mapestmin, mapestmax, len(mapestpmf))
        # maptrupmf = maptrupmf / sum(mapestpmf)
        # mapestpmf = mapestpmf / sum(mapestpmf)
        # plt.figure()
        # plt.plot(maptruval, maptrupmf, 'g')
        # plt.plot(mapestval, mapestpmf, 'b')
        # plt.show()
        # pass


class ModelsEnsemble():
    def __init__(self, title):
        self.title = title
        self.runs = []

    def runningmean(self, x, n):
        ypadded = np.pad(x, (n // 2, n - 1 - n // 2), mode='edge')
        return np.convolve(ypadded, np.ones((n,)) / n, mode='valid')

    def finalize(self):
        self.ensx = []
        self.ensy = []
        for log in self.runs:
            log = np.asarray(log)
            self.ensx.append(log[:, 1])
            self.ensy.append(log[:, 2])
        self.ensx = np.asarray(self.ensx)
        self.ensy = np.asarray(self.ensy)
        self.ensxmean = self.runningmean(np.mean(self.ensx, axis=0), 20)
        self.ensymean = self.runningmean(np.mean(self.ensy, axis=0), 20)
        self.ensxstd = self.runningmean(np.std(self.ensx, axis=0), 20)
        self.ensystd = self.runningmean(np.std(self.ensy, axis=0), 20)
        pass

    def plot(self):
        self.finalize()
        lw = 1
        plt.figure()
        plt.subplot(2, 1, 1)
        for log in self.runs:
            log = np.asarray(log)
            plt.plot(log[:, 0], log[:, 1], linewidth=lw, color='g', alpha=.25)
            plt.plot(log[:, 0], self.ensxmean, linewidth=lw, color='b', alpha=.1)
            plt.plot(log[:, 0], self.ensxmean + 1.96 * self.ensxstd, linewidth=lw, color='b', alpha=.05)
            plt.plot(log[:, 0], self.ensxmean - 1.96 * self.ensxstd, linewidth=lw, color='b', alpha=.05)
            plt.ylabel('x')
            if self.title: plt.title(self.title)
        plt.subplot(2, 1, 2)
        for log in self.runs:
            log = np.asarray(log)
            plt.plot(log[:, 0], log[:, 2], linewidth=lw, color='g', alpha=.25)
            plt.plot(log[:, 0], self.ensymean, linewidth=lw, color='b', alpha=.1)
            plt.plot(log[:, 0], self.ensymean + 1.96 * self.ensystd, linewidth=lw, color='b', alpha=.05)
            plt.plot(log[:, 0], self.ensymean - 1.96 * self.ensystd, linewidth=lw, color='b', alpha=.05)
            plt.ylabel('y')

def rccircuit(runs):
    from classical import Kalman
    ens = DecisionsEns()
    for runndx in range(runs):
        tracker = Kalman(mode='rccircuit', plot=False, signal=300.)
        dec = Decisions(mode='rccircuit', tracker=tracker)
        ens.addsig(decisionfunction=dec.decisionfunction)
        tracker = Kalman(mode='rccircuit', plot=False, signal=0.)
        dec = Decisions(mode='rccircuit', tracker=tracker)
        ens.addnoise(decisionfunction=dec.decisionfunction)
    ens.finalize()
    return ens

def rccircuit_ensemble(runs, signal, title=None):
    from rccircuit import Rccircuit
    ens = ModelsEnsemble(title)
    for runndx in range(runs):
        sim = Rccircuit(signal)
        for step in sim.steps(): continue
        ens.runs.append(sim.log)
    return ens

if __name__ == "__main__":
    ens = rccircuit(runs=100)
    ens.plot_decisionfunctions()
    ens.plot_roc()
    plt.show()

    enssig = rccircuit_ensemble(runs=10, signal=300., title='signal')
    ensnoise = rccircuit_ensemble(runs=10, signal=0., title='noise')
    enssig.plot()
    ensnoise.plot()
    plt.show()
    pass

