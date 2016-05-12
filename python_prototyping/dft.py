import numpy as np
import matplotlib.pyplot as plt
from scipy.io.wavfile import read

def DFT(x, N):
  X = np.array([])
  k_indices = np.arange(-N/2, N/2)
  for k in k_indices:
    s = np.exp(1j * 2 * np.pi * k / N * k_indices)
    X = np.append(X, np.dot(x[0:N], np.conjugate(s)))
  return X

def invDFT(X, N):
  y = np.array([])
  n_indices = np.arange(-N/2, N/2)
  for n in n_indices:
    s = np.exp(1j * 2 * np.pi * n / N * n_indices)
    y = np.append(y, 1.0 / N * np.dot(X, np.conjugate(s)))
  return y

'''
Beyond here shouldn't be used as part of the module, only for testing
'''
def showDFT(X, N):
  print X
  nindices = np.arange(-N/2, N/2)
  maxVal = np.max(X)
  minVal = np.min(X)
  plt.plot(nindices, np.abs(X))
  plt.axis([-N/2, N/2, minVal, maxVal])
  plt.xlabel('frequency')
  plt.ylabel('magnitude')
  plt.show()

def showWave(x, xlims, ylims, increment):
  indices = np.arange(xlims[0], xlims[1], increment)
  plt.plot(indices, np.real(x))
  plt.axis([xlims[0], xlims[1], ylims[0], ylims[1]])
  plt.xlabel('time')
  plt.ylabel('magnitude')
  plt.show()

def testDFT():
  N = 64
  k0 = 7
  x = np.exp(1j * 2 * np.pi * k0 / N * np.arange(N))
  X = DFT(x, N)
  showDFT(X, N)

def testInvDFT():
  N = 64
  k0 = 7
  x = np.exp(1j * 2 * np.pi * k0 / N * np.arange(N))
  X = DFT(x, N)
  x = invDFT(X, N)
  showWave(x, [-N/2, N/2], [-1, 1], 1)

def testDFTWithFlute():
  #(fs, x) = read('../sounds_wav/clarinet/clarinet_B5_1_forte_normal.wav')
  #(fs, x) = read('flute_A4_05_forte_normal.wav')
  (fs, x) = read('../sounds_wav/violin/violin_A4_05_forte_arco-normal.wav')

  t = np.arange(x.size)/float(fs)
  X = DFT(x, 10000)
  showDFT(X, 10000)

if __name__ == '__main__':
  testDFTWithFlute()
