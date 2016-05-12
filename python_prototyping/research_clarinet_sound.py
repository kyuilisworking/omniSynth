import numpy as np
import matplotlib.pyplot as plt
from scipy.io.wavfile import read

#(fs, x) = read('../sounds_wav/clarinet/clarinet_B5_1_forte_normal.wav')
#(fs, x) = read('../sounds_wav/violin/violin_A4_05_forte_arco-normal.wav')
(fs, x) = read('../sounds_wav/flute/flute_A4_05_forte_normal.wav')

print x.size
x = x[0:5000]
X = np.fft.rfft(x)
X = X.imag
print X.size
plt.plot(X)
plt.show()

