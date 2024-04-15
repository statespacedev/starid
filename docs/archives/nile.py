import matplotlib.pyplot as plt
import numpy as np
import math


def main():
    model = Model()
    model.filtering()
    model.state_smoothing()
    model.simulation()
    model.plot()
    plt.show()


# inverse-gamma distributions of the disturbance variances sigsqrepsilon and sigsqreta
class Params():
    def __init__(self):
        self.g1 = [3, .02]
        self.g2 = [3, .02]

    def path(self, epsilons, etas):
        self.theta1 = [1e-2, 1e-5]
        self.theta2 = [1e-2, 1e-5]
        N = len(epsilons)
        self.theta1[0] = self.theta1[0] + N / 2
        self.theta2[0] = self.theta2[0] + N / 2
        for j in range(1, N):
            self.theta1[1] += .5 * epsilons[j] ** 2
            self.theta2[1] += .5 * etas[j] ** 2


class Model():
    def __init__(self):
        self.ts = [float(rec[0]) for rec in nile()]
        self.y = [float(rec[1]) for rec in nile()]
        self.prep()
        self.sigsqrepsilon = 15099.
        self.sigsqreta = 1469.1
        self.a[0] = 1120.
        self.P[0] = 1e4

    def filtering(self):
        for t in range(len(self.y)):
            self.v[t] = self.y[t] - self.a[t]
            self.F[t] = self.P[t] + self.sigsqrepsilon
            self.K[t] = self.P[t] / self.F[t]
            if t < len(self.y) - 1:
                self.a[t + 1] = self.a[t] + self.K[t] * self.v[t]
                self.P[t + 1] = self.P[t] * (1 - self.K[t]) + self.sigsqreta

    def state_smoothing(self):
        for t in range(len(self.y) - 1, -1, -1):
            self.r[t - 1] = (1 / self.F[t]) * self.v[t] + (1 - self.K[t]) * self.r[t]
            self.alphahat[t] = self.a[t] + self.P[t] * self.r[t - 1]
            self.N[t - 1] = (1 / self.F[t]) + (1 - self.K[t]) ** 2 * self.N[t]
            self.V[t] = self.P[t] - self.P[t] ** 2 * self.N[t - 1]

    def disturbance_smoothing(self, y):
        v = np.zeros(len(y))
        r = np.zeros(len(y))
        epsilonhat = np.zeros(len(y))
        for t in range(len(v)): v[t] = y[t] - self.a[t]
        for t in range(len(r) - 1, -1, -1): r[t - 1] = (1 / self.F[t]) * v[t] + (1 - self.K[t]) * r[t]
        for t in range(len(epsilonhat)):
            u = (1 / self.F[t]) * v[t] - self.K[t] * r[t]
            epsilonhat[t] = self.sigsqrepsilon * u
        return epsilonhat

    def simulation(self):
        epsilonhat = self.disturbance_smoothing(self.y)
        etaplus = math.sqrt(self.sigsqreta) * np.random.normal(0., 1., 100)
        epsilonplus = math.sqrt(self.sigsqrepsilon) * np.random.normal(0., 1., 100)
        alphaplus = np.zeros(len(self.y))
        yplus = np.zeros(len(self.y))
        alphaplus[0] = self.y[0]
        for t in range(len(self.y)):
            yplus[t] = alphaplus[t] + epsilonplus[t]
            if t < len(self.y) - 1:
                alphaplus[t + 1] = alphaplus[t] + etaplus[t]
        epsilonplushat = self.disturbance_smoothing(yplus)
        for t in range(len(self.y)):
            self.epsilontil[t] = epsilonplus[t] + (epsilonhat[t] - epsilonplushat[t])
            self.alphatil[t] = self.y[t] - self.epsilontil[t]
            if t < len(self.y) - 1:
                self.etatil[t] = self.alphatil[t + 1] - self.alphatil[t]

    def prep(self):
        self.a = np.zeros(len(self.y))
        self.P = np.zeros(len(self.y))
        self.v = np.zeros(len(self.y))
        self.F = np.zeros(len(self.y))
        self.K = np.zeros(len(self.y))
        self.r = np.zeros(len(self.y))
        self.alphahat = np.zeros(len(self.y))
        self.N = np.zeros(len(self.y))
        self.V = np.zeros(len(self.y))
        self.epsilontil = np.zeros(len(self.y))
        self.alphatil = np.zeros(len(self.y))
        self.etatil = np.zeros(len(self.y))

    def plot(self):
        plt.figure()
        plt.plot(self.ts, self.a, c='b', linewidth=1, alpha=.25)
        plt.plot(self.ts, self.a + 2. * np.sqrt(np.asarray(self.P)), c='b', linewidth=1, alpha=.1)
        plt.plot(self.ts, self.a - 2. * np.sqrt(np.asarray(self.P)), c='b', linewidth=1, alpha=.1)
        plt.plot(self.ts, self.alphahat, c='g', linewidth=1)
        plt.plot(self.ts, self.alphahat + 2. * np.sqrt(np.asarray(self.V)), c='g', linewidth=1, alpha=.5)
        plt.plot(self.ts, self.alphahat - 2. * np.sqrt(np.asarray(self.V)), c='g', linewidth=1, alpha=.5)
        plt.scatter(self.ts, self.alphatil, c='g', s=5)  # sample from state dist conditional on obs
        plt.scatter(self.ts, self.y, c='b', s=5, alpha=.25)


def nile(): return [[1871, 1120], [1872, 1160], [1873, 963], [1874, 1210], [1875, 1160], [1876, 1160], [1877, 813],
                    [1878, 1230], [1879, 1370], [1880, 1140], [1881, 995], [1882, 935], [1883, 1110], [1884, 994],
                    [1885, 1020], [1886, 960], [1887, 1180], [1888, 799], [1889, 958], [1890, 1140], [1891, 1100],
                    [1892, 1210], [1893, 1150], [1894, 1250], [1895, 1260], [1896, 1220], [1897, 1030], [1898, 1100],
                    [1899, 774], [1900, 840], [1901, 874], [1902, 694], [1903, 940], [1904, 833], [1905, 701],
                    [1906, 916], [1907, 692], [1908, 1020], [1909, 1050], [1910, 969], [1911, 831], [1912, 726],
                    [1913, 456], [1914, 824], [1915, 702], [1916, 1120], [1917, 1100], [1918, 832], [1919, 764],
                    [1920, 821], [1921, 768], [1922, 845], [1923, 864], [1924, 862], [1925, 698], [1926, 845],
                    [1927, 744], [1928, 796], [1929, 1040], [1930, 759], [1931, 781], [1932, 865], [1933, 845],
                    [1934, 944], [1935, 984], [1936, 897], [1937, 822], [1938, 1010], [1939, 771], [1940, 676],
                    [1941, 649], [1942, 846], [1943, 812], [1944, 742], [1945, 801], [1946, 1040], [1947, 860],
                    [1948, 874], [1949, 848], [1950, 890], [1951, 744], [1952, 749], [1953, 838], [1954, 1050],
                    [1955, 918], [1956, 986], [1957, 797], [1958, 923], [1959, 975], [1960, 815], [1961, 1020],
                    [1962, 906], [1963, 901], [1964, 1170], [1965, 912], [1966, 746], [1967, 919], [1968, 718],
                    [1969, 714], [1970, 740]]


if __name__ == "__main__":
    main()
