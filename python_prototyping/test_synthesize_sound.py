from synthesize_sound import Synthesize_Sound_Freq
import matplotlib.pyplot as plt
import pyaudio
import time
import numpy as np
import threading

WIDTH = 100
NO_DATA_PER_WIDTH = 1024
CHANNELS = 2
RATE = 44100

p = pyaudio.PyAudio()

#freqs = np.random.sample(20) * 10
freqs = [1]
ss = Synthesize_Sound_Freq(freqs)

pitch = 440
freqs = np.random.sample(20) * 10
ss.set_freq_spectrum(freqs)
wave = ss.generate_wave(RATE, pitch, WIDTH*NO_DATA_PER_WIDTH)
current_data = wave.astype(np.float32).tostring()
#current_data = None


class myThread (threading.Thread):
    def __init__(self, threadID, name):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name

    def run(self):
        while True:
            global pitch
            global freqs
            global current_data
            ss.set_freq_spectrum(freqs)
            wave = ss.generate_wave(RATE, pitch, WIDTH*NO_DATA_PER_WIDTH)
            current_data = wave.astype(np.float32).tostring()

#thread1 = myThread(1, "Update_wave")

def callback(in_data, frame_count, time_info, status):
    #audio_data = np.fromstring(in_data, dtype=np.int16)
    #print pitch_detect.detect_pitch(audio_data, RATE)
    return (current_data, pyaudio.paContinue)

# Open stream with correct settings
stream = p.open(format=pyaudio.paFloat32,
                channels=CHANNELS,
                rate=RATE,
                output=True,
                stream_callback=callback)


stream.start_stream()
#thread1.start()

while stream.is_active():
    ss.set_freq_spectrum(freqs)
    wave = ss.generate_wave(RATE, pitch, WIDTH*NO_DATA_PER_WIDTH)
    current_data = wave.astype(np.float32).tostring()
    time.sleep(0.1)

stream.stop_stream()
stream.close()

p.terminate()
