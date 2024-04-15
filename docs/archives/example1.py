import numpy as np

# python lists
data = [2.1, 2.6, 2.3, 2.5, 2.2, 8.1, 8.3, 8.2, 8.6, 8.3]
w1 = [10, 10, 10, 10, 10, 1, 1, 1, 1, 1] # weight vector 1
w2 = [1, 1, 1, 1, 1, 10, 10, 10, 10, 10] # weight vector 2

# convert them to numpy arrays. these act as linear algebra vectors.
data = np.array(data)
w1 = np.array(w1)
w2 = np.array(w2)

# normalize the weight vectors, making them unit-vectors (norm or "length" 1.0)
# force the sum of the weights to be 1.0
uw1 = w1 / np.sum(w1)
uw2 = w2 / np.sum(w2)
# equivalently, force the L1 norm to be 1.0
uw1b = w1 / np.linalg.norm(w1, ord=1)
uw2b = w2 / np.linalg.norm(w2, ord=1)

# weighted average is the dot product of a normalized weight vector and a data vector
wa1 = np.dot(uw1, data)
wa2 = np.dot(uw2, data)
# here's an equivalent provided by numpy
wa1b = np.average(data, weights=uw1)
wa2b = np.average(data, weights=uw2)
# and np.average actually does normalize the weights internally, so you can skip normalization and do
wa1c = np.average(data, weights=w1)
wa2c = np.average(data, weights=w2)

pass

