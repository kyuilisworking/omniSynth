from synthesize_sound import Synthesize_Sound_Freq
import matplotlib.pyplot as plt
import pyaudio
import time
import numpy as np

WIDTH = 10
NO_DATA_PER_WIDTH = 1024
CHANNELS = 2
RATE = 44100
PITCH = 440


#freqs = [11, 8, -1.4, 1.4, 2.0, 1, 1.2, 0.8, 0.4]
#freqs = [10, 4, 0, 8, 7, 0, 2, 1, 0, 9, 2, 0, 2, 1]
#freqs = [10,8,0, 4, 0, 0, 0, 5, 0, 0, 0, 0, 4, 4, 4, 4]
freqs = [(440, 10), (882, 5.9), (436, 4.1), (447, 2), (877, 1.8), (22, 1.7), (443, 1.6), (1323, 1.4), (887, 1.1), (2201, 1), (24.9, 0.9), (1769, 0.7), (18.2, 0.7), (2196, 0.7), (14.8, 0.7), (1771, 0.7), (1758, 0.6), (13.1, 0.6), (2214, 0.6)]
#freqs = [(440, 10), (882, 5.9), (877, 1.8), (22, 1.7), (1323, 1.4), (2201, 1), (1769, 0.7), (14.8, 0.7), (13.1, 0.6)]
#freqs = [(440, 10), (882, 5.9), (877, 1.8), (1323, 1.4), (2201, 1), (1769, 0.7)]
#freqs = [(440, 10), (880, 0.1), (1320, 5), (1760, 0.1), (2200, 3), (2640, 0)]
freqs = [(440, 10)]
ss = Synthesize_Sound_Freq(freqs)

ss.set_freq_spectrum(freqs)
wave = ss.generate_wave(RATE, PITCH, WIDTH*NO_DATA_PER_WIDTH)
current_data = wave.astype(np.float32).tostring()


p = pyaudio.PyAudio()

# Open stream with correct settings
stream = p.open(format=pyaudio.paFloat32,
                channels=CHANNELS,
                rate=RATE,
                output=True)


while True:
    stream.write(current_data)
 
'''
while stream.is_active():
    ss.set_freq_spectrum(freqs)
    wave = ss.generate_wave(RATE, pitch, WIDTH*NO_DATA_PER_WIDTH)
    current_data = wave.astype(np.float32).tostring()
    time.sleep(0.1)
'''
stream.stop_stream()
stream.close()

p.terminate()
