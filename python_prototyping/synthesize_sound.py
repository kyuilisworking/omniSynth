import numpy as np

class Synthesize_Sound_Freq:

    def __init__(self, freq_spectrum=None):
        '''
        Constructor
        Arguments:
            freqs - python list of tuples (freq, amplitude)
        '''
        self.freq_spectrum = freq_spectrum
        self.current_phase = 0

    def set_freq_spectrum(self, freq_spectrum):
        self.freq_spectrum = freq_spectrum


    def generate_wave(self, sampling_rate, pitch, window_length):
        if (pitch != None):
            '''
            wave = np.zeros(window_length)

            #freqs = self.freqs_modulated_by_pitch(pitch)

            no_freqs = len(self.freq_spectrum)
            for j in range(no_freqs):
                amp = self.freq_spectrum[j]
                #freq = freqs[j]
                freq = self.freq_spectrum
                sinusoid = self.get_sinusoid_vals_window(sampling_rate, amp, freq, window_length, self.current_phase)
                cosinusoid = self.get_cosinusoid_vals_window(sampling_rate, amp, freq, window_length, self.current_phase)
                wave += sinusoid
            self.current_phase += 2*np.pi * freq / float(sampling_rate) * window_length
            total_periods = int(self.current_phase / (2 * np.pi))
            self.current_phase -= total_periods * 2 * np.pi
            return wave / no_freqs
            '''
            wave = np.zeros(window_length)


            no_freqs = len(self.freq_spectrum)
            for (freq, amp) in self.freq_spectrum:
                sinusoid = self.get_sinusoid_vals_window(sampling_rate, amp, freq, window_length, self.current_phase)
                wave += sinusoid
            self.current_phase += 2*np.pi * freq / float(sampling_rate) * window_length
            total_periods = int(self.current_phase / (2 * np.pi))
            self.current_phase -= total_periods * 2 * np.pi
            return wave / no_freqs
        else:
            return None

    def freqs_modulated_by_pitch(self, pitch):
        n = len(self.freq_spectrum)
        freqs = np.zeros(n)
        for i in range(n):
            freqs[i] = pitch * (1 + i)
        return freqs

    def get_sinusoid_vals_window(self, sampling_rate, amplitude, freq, window_length, init_phase=0):
        sinusoid = np.zeros(window_length)
        current_phase = init_phase

        dPhase = 2*np.pi * freq / float(sampling_rate)

        for i in range(window_length):
            sinusoid[i] = amplitude * np.sin(current_phase)
            current_phase += dPhase

        return sinusoid

    def get_cosinusoid_vals_window(self, sampling_rate, amplitude, freq, window_length, init_phase=0):
        cosinusoid = np.zeros(window_length)
        current_phase = init_phase

        dPhase = 2*np.pi * freq / float(sampling_rate)

        for i in range(window_length):
            cosinusoid[i] = amplitude * np.cos(current_phase)
            current_phase += dPhase

        return cosinusoid
    
