from ghmm import *
from p21_data import *

sigma = Alphabet(['a','t','g','c'])

edist = []
counter = 0
for base in counts:
    edist.append([])
    for c in counts[counter]:
        edist[counter].append(c/14.)
    counter += 1

# Emissions in DNA state
edist.append([0.25, 0.25, 0.25, 0.25])
print len(edist)

A = [ [ 0 for y in range(0, 11)] for x in range(0, 11)]

for i in range(0, 10):
    A[i][i+1] = 1

# Probability of staying in DNA or leaving
A[10][10] = 0.5
A[10][0] = 0.5


# Starting probabilities
pi = [ 0 for x in range(0, 11)]
pi[0] = 0.5
pi[10] = 0.5

m = HMMFromMatrices(sigma, DiscreteDistribution(sigma), A, edist, pi)

seq1 = m.sampleSingle(20)

print seq1
print seq1.getGeneratingStates()
