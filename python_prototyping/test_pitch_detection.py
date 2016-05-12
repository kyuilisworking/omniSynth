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
import synthesize_sound

WIDTH = 2
NO_DATA_PER_WIDTH = 1024
CHANNELS = 2
RATE = 88200

p = pyaudio.PyAudio()
#vis_data = visualize_data.Visualize_Data(WIDTH * NO_DATA_PER_WIDTH, (-10000, 10000))
#vis_data_auto = visualize_data.Visualize_Data(100, (-100000, 100000))
#vis_data_check = visualize_data.Visualize_Data(WIDTH * NO_DATA_PER_WIDTH, (-10000, 10000))

#pitch_detect = pitch_detection.Detect_Pitch('autocorrelation')
pitch_detect = pitch_detection.Detect_Pitch('hps_ccf')
ss = synthesize_sound.Synthesize_Sound_Freq([10])

def callback(in_data, frame_count, time_info, status):
    audio_data = np.fromstring(in_data, dtype=np.int16)
    pitch = pitch_detect.detect_pitch(audio_data, RATE)
    print pitch
    
    return (audio_data, pyaudio.paContinue)

stream = p.open(format=p.get_format_from_width(WIDTH),
                channels=CHANNELS,
                rate=RATE,
                input=True,
                stream_callback=callback)

stream.start_stream()
#vis_data.visualize()
#vis_data_auto.visualize()

while stream.is_active():
    time.sleep(0.1)

stream.stop_stream()
stream.close()

p.terminate()
