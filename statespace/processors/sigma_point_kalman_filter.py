"""sigma-point kalman filter"""
import math
import numpy as np
# noinspection PyProtectedMember
from scipy.linalg.blas import drot, drotg


class SigmaPoint:
    """modern sigma-point deterministic sampling kalman filter"""

    def __init__(self, conf):
        self.conf, self.log = conf, []
        if conf.factorized:
            self.run = self.spkf_factorized
        else:
            self.run = self.spkf

    def spkf(self, model):
        """sigma-point deterministic sampling kalman filter"""
        (sim, f, h), (F, H, R, Q, G, x, P) = model.entities(), model.ekf.entities()
        XY, W, WM = model.spkf.entities()
        for t, o, u in sim():
            X, Y = XY(x, P, u)
            x, y = np.sum(np.multiply(WM, X), axis=1).reshape(-1, 1), np.sum(np.multiply(W, Y), axis=1)[0]
            Xres, Yres = self.Xres(X, x), self.Yres(Y, y)
            P = np.multiply(WM, Xres) @ Xres.T + G @ Q @ G.T
            K = np.multiply(WM, Xres) @ Yres.T / (np.multiply(W, Yres) @ Yres.T + R)
            x = x + K * (o - y)
            P = P - K @ (np.multiply(W, Yres) @ Yres.T + R) @ K.T
            self.log.append([t, x, y])

    def spkf_factorized(self, model):
        """cholesky factorized sigma-point sampling kalman filter"""
        (sim, f, h), (F, H, R, Q, G, x, P) = model.entities(), model.ekf.entities()
        XY, W, X2, Y2, P2, S, Sp, So = model.spkf.entities_cholesky()
        for t, o, u in sim():
            x, S, X, Y = self.temporal(x, XY, W, X2, S, Sp, u)
            x, S, y = self.observational(x, o, X, Y, W, X2, Y2, P2, S, So)
            self.log.append([t, x, y])

    def temporal(self, x, XY, W, Xtil, S, Sproc, u):
        """cholesky factorized temporal update"""
        X, Y = XY(x, S, u)
        x = X @ W.T
        for i in range(X.shape[1]): Xtil[:, i] = (X[:, i].reshape(-1, 1) - x).T
        q, r = np.linalg.qr(np.concatenate([math.sqrt(W[0, 1]) * Xtil[:, 1:], Sproc], 1))
        S = self.cholesky_update(r.T[0:X.shape[0], 0:X.shape[0]], W[0, 0] * Xtil[:, 0].reshape(-1, 1))
        return x, S, X, Y

    def observational(self, x, o, X, Y, W, Xtil, Ytil, Pxy, S, Sobs):
        """cholesky factorized observational update"""
        y = (W @ Y.T)[0, 0]
        for i in range(X.shape[1]): Ytil[0, i] = (Y[0, i].reshape(-1, 1) - y).T
        q, r = np.linalg.qr(np.concatenate([math.sqrt(W[0, 1]) * Ytil[:, 1:], Sobs], 1))
        Sy = self.cholesky_update(r.T[0:1, 0:1], W[0] * Ytil[:, 0].reshape(-1, 1))
        for i in range(X.shape[1]):
            tmp = W[0, i] * Xtil[:, i].reshape(-1, 1) * Ytil[0, i]
            Pxy += tmp
        if Sy[0, 0] < math.sqrt(10) or Sy[0, 0] > math.sqrt(1000): Sy[0, 0] = math.sqrt(1000)
        K = Pxy / Sy[0, 0] ** 2
        U = K * Sy
        x = x + K * (o - y)
        S = self.cholesky_downdate(S, U)
        return x, S, y

    @staticmethod
    def cholesky_update(R, z):
        """cholesky update"""
        n = z.shape[0]
        for k in range(n):
            c, s = drotg(R[k, k], z[k])
            drot(R[k, :], z, c, s, overwrite_x=True, overwrite_y=True)
        return R

    @staticmethod
    def cholesky_downdate(R, z):
        """cholesky downdate"""
        n = R.shape[0]
        for k in range(n):
            if (R[k, k] - z[k]) * (R[k, k] + z[k]) < 0: return R
            rbar = np.sqrt((R[k, k] - z[k]) * (R[k, k] + z[k]))
            for j in range(k + 1, n):
                R[k, j] = 1. / rbar * (R[k, k] * R[k, j] - z[k] * z[j])
                z[j] = 1. / R[k, k] * (rbar * z[j] - z[k] * R[k, j])
            R[k, k] = rbar
        return R

    @staticmethod
    def Xres(X, x):
        X2 = X.copy()
        for i in range(X2.shape[1]):
            for j in range(X2.shape[0]): X2[j, i] -= x[j, 0]
        return X2

    @staticmethod
    def Yres(Y, y):
        for i in range(Y.shape[1]): Y[0, i] -= y
        return Y


if __name__ == "__main__":
    pass
