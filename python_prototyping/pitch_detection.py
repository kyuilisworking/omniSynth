import numpy as np
import filters
import scipy.signal as signal
import matplotlib.pyplot as plt
from scipy.io.wavfile import read

class Detect_Pitch:

    def __init__(self, detection_method='autocorrelation'):
        '''
        Constructor
        Arguments:
            detection_method is a string - currently only supports autocorrelation
        '''
        self.detection_method = detection_method
        self.data = None


    def set_data(self, data):
        '''
        Sets the data
        Arguments:
            data is a nx1 numpy matrix
        '''
        self.data = data

    def detect_pitch(self, data, sampling_rate, filter_freq=1000):
        '''
        Detects pitch
        Arguments:
            data is a nx1 numpy matrix
            sampling_rate is the sampling rate of the data
        '''
        self.data = data
        options = {
            'autocorrelation': self.detect_pitch_autocorrelation,
            'hps': self.detect_pitch_hps,
            'hps_ccf': self.detect_pitch_hps_ccf_kyuil
        }
        return options[self.detection_method](data, sampling_rate)
        
    def autocorrelate(self, data, shift):
        '''
        Finds the autocorrelation coefficient for the data with specified shift
        Arguments:
            data - numpy array with size nx1
            shift - constant value to specify the lag of the data
        '''
        n = data.size
        autocoeff = 0
        for i in range(0, n - shift):
            autocoeff += float(data[i])*float(data[i + shift])
        return autocoeff / 100000000

    def detect_pitch_autocorrelation(self, data, sampling_rate, max_width=500, filter_freq=1200):
        '''
        Detects the pitch using the autocorrelation function
        Arguments:
            data - numpy array with size nx1
            sampling_rate - the sampling rate of data
            max_width - the maximum width that I check in the data
        '''
        data = filters.low_pass(data, filter_freq, sampling_rate)

        original_autocoeff = self.autocorrelate(data, 0)
        epsilon = 0.9 * original_autocoeff

        current_shift = 10
        pitch_detected = False
        previous_coeff = self.autocorrelate(data, current_shift)
        n = data.size
        #for initializing previous_coeff to reduce load on iteration
        previous_derivative = -1
        current_shift += 1

        while (not pitch_detected) and (current_shift < max_width):
            autocoeff = self.autocorrelate(data, current_shift)
            current_derivative = autocoeff - previous_coeff
            if (current_derivative < 0 and previous_derivative >= 0):
                #peak found
                pitch_detected = True
            else:
                previous_derivative = current_derivative
                previous_coeff = autocoeff
                current_shift += 10

        
        if pitch_detected == True:
            #fine tuning
            pitch_detected = False
            previous_derivative = current_derivative
            previous_coeff = autocoeff
            current_shift -= 1


            while (not pitch_detected) and (current_shift < max_width):
                autocoeff = self.autocorrelate(data, current_shift)
                current_derivative = previous_coeff - autocoeff
                if (current_derivative >= 0 and previous_derivative < 0):
                    #peak found
                    pitch_detected = True
                else:
                    previous_derivative = current_derivative
                    previous_coeff = autocoeff
                    current_shift -= 1
                   
            current_shift += 1

        if not pitch_detected:
            print "Error: window too small to detect pitch."
            print "Please incrase the sampling window."
            return None
        else:
            #find the actual frequency using the sampling_rate
            return sampling_rate / float(current_shift)

    def detect_max_idx(self, data):
        current_max_val = 0
        current_max_idx = 0
        n = data.size

        for i in range(0, n):
            if data[i] > current_max_val:
                current_max_val = data[i]
                current_max_idx = i
        return current_max_idx

    def detect_second_max(self, data):
        current_max_val = 0
        current_max_idx = 0
        current_sec_max_val = 0
        current_sec_max_idx = 0

        n = data.size

        for i in range(n):
            if data[i] > current_max_val:
                current_sec_max_val = current_max_val
                current_sec_max_idx = current_max_idx
                current_max_val = data[i]
                current_max_idx = i
        return current_sec_max_idx



    def moving_average_filter(self, data, k):
        n = data.size
        average = np.zeros(n);
        print k

        for i in range(n):
            s = 0
            for j in range(1, k):
                if i - j >= 0:
                    s += data[i - j]
                if i + j < n:
                    s += data[i + j]
            print s
            average[i] = s / float(2 * k + 1)

        return average

    def downsample(self, data, n, sampling_rate):
        N = data.size
        average = filters.low_pass(data, 800, sampling_rate)
        #down sample the averaged data
        down_sampled_data = np.zeros(N / 2)

        for i in range(N / 2):
            down_sampled_data[i] = average[i * 2]
        return down_sampled_data


    def detect_pitch_hps(self, data, sampling_rate):
        freq_spectrum = np.fft.rfft(data)
        freq_spectrum = np.absolute(freq_spectrum)
        fft1 = freq_spectrum / float(100000)
        fft2 = signal.decimate(fft1, 2)
        fft3 = signal.decimate(fft1, 3)
        fft4 = signal.decimate(fft1, 4)
        fft5 = signal.decimate(fft1, 5)
        fft6 = signal.decimate(fft1, 6)

        max1 = self.detect_max_idx(fft1)
        max2 = self.detect_max_idx(fft2)
        sec_max1 = self.detect_second_max(fft1)
        sec_max2 = self.detect_second_max(fft2)

        print "maxes"
        print max1 * float(sampling_rate) / data.size
        print max2

        print "second maxes"
        print sec_max1
        print sec_max2
        
        plt.plot(fft1)
        plt.plot(fft2)
        plt.plot(fft3)
        

        n = fft6.size
        hps = np.zeros(n)

        for i in range(n):
            hps[i] = fft1[i] * fft2[i] * fft3[i] * fft4[i] * fft5[i] * fft6[i]

        #plt.plot(hps)
        plt.axis([0, 300, 0, 30])
        plt.show()

        maxFreqIdx1 = 0
        maxFreqIdx2 = 0
        maxFreqAmp1 = 0
        maxFreqAmp2 = 0
        
        for i in range(n):
            if hps[i] > maxFreqAmp1:
                maxFreqIdx2 = maxFreqIdx1
                maxFreqAmp2 = maxFreqAmp1
                maxFreqIdx1 = i
                maxFreqAmp1 = hps[i]

        #N = data.size
        return sampling_rate / maxFreqIdx1

    def ccf_hps(self, data1, data2, shift):
        '''
        Calculates the cross coefficient
        shift is the shift of data2
        assumes that data2.size <= data1.size / 2
        '''
        ccf = 0
        if shift < 0:
            shift = -shift
            for i in range(data2.size - shift):
                ccf += data1[i] * data2[i + shift]
        else:
            for i in range(data2.size):
                ccf += data1[i + shift] * data2[i]
        return ccf

    def get_hps(self, freq_spectrum, downsample, sampling_rate):
        '''
        Takes in fft of original signal, returns the hps of the
        signal and the downsampled signal
        '''
        fft1 = freq_spectrum / float(1000000)
        #fft2 = signal.decimate(fft1, downsample, ftype='iir')
        fft2 = self.downsample(fft1, downsample, sampling_rate)
        #fft2 = self.downsample(fft1, downsample)

        n = fft2.size
        hps = np.zeros(n)

        #since the downsampled and original signals don't exactly line up,
        #use autocorrelation to figure out this shift
        max_shift = 4

        best_cross_coeff = 0
        best_shift = 0

        for i in range(max_shift):
            ccfl = self.ccf_hps(fft1, fft2, -i)
            ccfr = self.ccf_hps(fft1, fft2, i)

            if ccfl > ccfr:
                if ccfl > best_cross_coeff:
                    best_cross_coeff = ccfl
                    best_shift = -i
            else:
                if ccfr > best_cross_coeff:
                    best_cross_coeff = ccfr
                    best_shift = i
        '''
        plt.plot(fft1)
        plt.plot(fft2)

        plt.axis([0, 1000, 0, 10])
        plt.show()
        '''
        #now calculate the hps
        hps = np.zeros(fft2.size)
        shift = best_shift

        if shift < 0:
            shift = -shift
            for j in range(fft2.size - shift):
                hps[j] = fft1[j] * fft2[j + shift]
        else:
            for j in range(fft2.size):
                hps[j] = fft1[j + shift] * fft2[j]

        return hps

    def detect_pitch_hps_ccf_kyuil(self, data, sampling_rate):
        '''
        Kyuil's algorithm for detecting pitch
        '''

        #data = filters.low_pass(data, 3000, sampling_rate)

        #first downsample the original signal, then the harmonics should
        #be closely lined up
        freq_spectrum = np.fft.fft(data)
        freq_spectrum[0] = 0
        freq_spectrum = np.absolute(freq_spectrum)

        no_of_hps = 4

        hps = []
        max_idxes = []
        max_vals = []

        for i in range(1, no_of_hps + 1):
            hps.append(self.get_hps(freq_spectrum, i + 1, sampling_rate))
            max_idxes.append(self.detect_max_idx(hps[i - 1]))
            max_vals.append(hps[i-1][max_idxes[i-1]])

        max_val_idx = self.detect_max_idx(np.array(max_vals))
        max_idx_hps = max_idxes[max_val_idx]

        return max_idx_hps / float(data.size) * sampling_rate
        

if __name__ == '__main__':
    pitch_detector = Detect_Pitch('hps')
    #(fs, x) = read('../sounds_wav/violin/violin_A4_05_forte_arco-normal.wav')
    #(fs, x) = read('../sounds_wav/clarinet/clarinet_A3_1_forte_normal.wav')    
    (fs, x) = read('../sounds_wav/flute/flute_A4_05_forte_normal.wav')    
    #(fs, x) = read('output.wav')  

    '''
    freq_spectrum = np.fft.fft(x[0:8000])
    freq_spectrum = np.absolute(freq_spectrum) / 100000

    down_freq_spectrum = pitch_detector.downsample(freq_spectrum, 5, fs)
    #low_passed_freq_spectrum = filters.low_pass(freq_spectrum, 500, fs)
    plt.plot(freq_spectrum)
    plt.plot(down_freq_spectrum)
    plt.axis([0, 1000, 0, 10])
    plt.show()
    '''

    '''
    
    data = x[2000:10000]
    data = filters.low_pass(data, 1200, fs)

    autocoeffs = np.zeros(200)
    for i in range(200):
        autocoeffs[i] = pitch_detector.autocorrelate(data, i)

    plt.plot(autocoeffs)
    plt.axis([0, 200, 0, 20])
    plt.show()
    '''

    print pitch_detector.detect_pitch(x[2000:10000], fs)
    
