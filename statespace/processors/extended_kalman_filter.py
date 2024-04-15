"""baseline extended kalman filters. both the standard textbook form, and the ud factorized form. 1957 - the space
race was on for real - why? the ability to move roughly a ton from anywhere to anywhere on the globe in under thirty
minutes. on top of that, add the capability to do it automatically, without human intervention - just in case - might
not be anybody left around to do any intervening.

a closed-loop system with its own internal sensors bringing data in from the outside world was needed - a black box -
and it needed to make reasonable decisions based on its sensors, using a model of the sensor errors and the outside
world. is a fully linear system going to work? no - it needs to adapt to nonlinearities in the models. a huge new
thing here is representing the uncertainties - by simple analogy with 'variance' and its square-root 'sigma',
at first, this is going to be a covariance matrix - a quadratic form ATAx with 'squared units' just like 'variance'.
the same idea appeared in finance via markowitz portfolio theory at exactly the same time - would be fascinating to
know how much direct contact between kalman and markowitz took place at the time? and all of this is only a few years
after dantzig and von neumann - how much direct contact did this whole group have?

the link between 'variance' and its square-root 'sigma' has already been mentioned - is that concept useful here?
over time, into the 60s and 70s, it very much was so - the 'factorized', 'square-root' forms appeared. they played a
crucial role in adapting to the then current hardware - the 16-bit apollo guidance computer required the
'square-root' ekf. this drove the applied mathematicians and electrical engineers onward from matrix inversion toward
a whole zoo of matrix factorizations - lu, qr, ud, svd, cholesky, etc."""
import numpy as np


class Kalman:
    """classical extended kalman filter"""

    def __init__(self, conf):
        self.conf, self.log = conf, []
        if conf.factorized:
            self.run = self.ekf_factorized
        else:
            self.run = self.ekf

    def ekf(self, model):
        """basic form"""
        (sim, f, h), (F, H, R, Q, G, x, P) = model.entities(), model.ekf.entities()
        for t, o, u in sim():
            x = f(x) + u
            y = h(x)
            P = F(x) @ P @ F(x).T + G @ Q @ G.T
            K = P @ H(x).T / (H(x) @ P @ H(x).T + R)
            x = x + K * (o - y)
            P = P - K @ H(x) @ P
            self.log.append([t, x, y])
        return True

    def ekf_factorized(self, model):
        """ud factorized form"""
        (sim, f, h), (F, H, R, Q, G, x, P) = model.entities(), model.ekf.entities()
        U, D = self.ud_factorization(P)
        for t, o, u in sim():
            x, U, D = self.temporal(f(x) + u, U, D, F(x), G, Q)
            x, U, D, y = self.observational(x, U, D, H(x), o, R, h(x))
            self.log.append([t, x, y])

    @staticmethod
    def ud_factorization(M):
        """ud factorization"""
        assert np.allclose(M, M.T)
        n, M = M.shape[0], np.triu(M)
        U, d = np.eye(n), np.zeros(n)
        for j in reversed(range(2, n + 1)):
            d[j - 1] = M[j - 1, j - 1]
            if d[j - 1] > 0:
                alpha = 1.0 / d[j - 1]
            else:
                alpha = 0.0
            for k in range(1, j):
                beta = M[k - 1, j - 1]
                U[k - 1, j - 1] = alpha * beta
                M[0:k, k - 1] = M[0:k, k - 1] - beta * U[0:k, j - 1]
        d[0] = M[0, 0]
        return U, np.diag(d)

    @staticmethod
    def temporal(xin, Uin, Din, Phi, Gin, Q):
        """thornton temporal update"""
        U, D, G, n, r = np.eye(len(xin)), Din, Gin, np.shape(Q)[0], np.shape(Q)[0]
        x, PhiU = Phi @ xin, Phi @ Uin
        for i in reversed(range(len(xin))):
            sigma = 0
            for j in range(n):
                sigma = sigma + PhiU[i, j] ** 2 * Din[j, j]
                if j <= r - 1: sigma = sigma + G[i, j] ** 2 + Q[j, j]
            D[i, i] = sigma
            ilim = i - 1
            if ilim > 0:
                for j in range(ilim):
                    sigma = 0
                    for k in range(n): sigma = sigma + PhiU[i, k] * Din[k, k] * PhiU[j, k]
                    for k in range(r): sigma = sigma + G[i, k] * Q[k, k] * G[j, k]
                    U[j, i] = sigma / D[i, i]
                    for k in range(n): PhiU[j, k] = PhiU[j, k] - U[j, i] * PhiU[i, k]
                    for k in range(r): G[j, k] = G[j, k] - U[j, i] * G[i, k]
        return x, U, D

    @staticmethod
    def observational(xin, Uin, Din, H, obs, R, yhat):
        """bierman observation update"""
        x, U, D, dz, alpha, gamma = xin, Uin, Din, obs - yhat, R, 1 / R
        a = U.T @ H.T
        b = D @ a
        for j in range(len(xin)):
            beta = alpha
            alpha = alpha + a[j] * b[j]
            lamda = -a[j] * gamma
            gamma = 1 / alpha
            D[j, j] = beta * gamma * D[j, j]
            jlim = j - 1
            if not jlim < 0:
                for i in range(jlim):
                    beta = U[i, j]
                    U[i, j] = beta + b[i] * lamda
                    b[i] = b[i] + b[j] * beta
        dzs = gamma * dz
        x = x + dzs * b
        return x, U, D, yhat


if __name__ == "__main__":
    pass
