"""
PyAudio Example: Make a wire between input and output (i.e., record a
few samples and play them back immediately).

This is the callback (non-blocking) version.
"""

import pyaudio
import visualize_data
import time
import numpy as np
import pitch_detection
import filters

WIDTH = 4
NO_DATA_PER_WIDTH = 1024
CHANNELS = 2
RATE = 44100

p = pyaudio.PyAudio()
vis_data = visualize_data.Visualize_Data(WIDTH * NO_DATA_PER_WIDTH, (0, 1000))

pitch_detect = pitch_detection.Detect_Pitch('autocorrelation')

def callback(in_data, frame_count, time_info, status):
    audio_data = np.fromstring(in_data, dtype=np.int16)
    vis_data.update_data(audio_data / 100)
    #print pitch_detect.detect_pitch(audio_data, RATE)
    return (in_data, pyaudio.paContinue)

stream = p.open(format=p.get_format_from_width(WIDTH),
                channels=CHANNELS,
                rate=RATE,
                input=True,
                output=False,
                stream_callback=callback)

stream.start_stream()
vis_data.visualize()

while stream.is_active():
    time.sleep(0.1)

stream.stop_stream()
stream.close()

p.terminate()
