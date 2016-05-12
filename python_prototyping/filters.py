import numpy as np
from scipy.signal import butter, lfilter

def butter_bandpass(lowcut, highcut, fs, order=5):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    b, a = butter(order, [low, high], btype='band')
    return b, a


def butter_bandpass_filter(data, lowcut, highcut, fs, order=5):
    b, a = butter_bandpass(lowcut, highcut, fs, order=order)
    y = lfilter(b, a, data)
    return y


def band_pass():
    return None

def low_pass(data, cutoff_freq, sampling_rate):
    dT = 1.0 / float(sampling_rate)
    RC = 1.0 / (2.0 * np.pi * float(cutoff_freq))
    alpha = dT / (RC + dT)

    n = data.size
    new_data = np.zeros(n)

    for i in range(1, n):
        new_data[i] = alpha * data[i] + (1.0 - alpha) * new_data[i - 1]

    dT1 = new_data[1] - new_data[2]
    new_data[0] = new_data[1] + dT1

    return new_data

def high_pass(data, cutoff_freq, sampling_rate):
    dT = 1.0 / float(sampling_rate)
    RC = 1.0 / (2.0 * np.pi * float(cutoff_freq))
    alpha = RC / (RC + dT)

    n = data.size
    new_data = np.zeros(n)

    for i in range(1, n):
        new_data[i] = alpha * new_data[i - 1] + alpha*(data[i] - data[i - 1])
    
    dT1 = new_data[1] - new_data[2]
    new_data[0] = new_data[1] + dT1

    return new_data

