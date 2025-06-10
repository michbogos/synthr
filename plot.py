import matplotlib.pyplot as plt
import numpy as np
import wave
import sys
from scipy.signal import ShortTimeFFT
from scipy.signal.windows import gaussian


spf = wave.open("string.wav", "r")

# Extract Raw Audio from Wav File
signal = spf.readframes(-1)
signal = np.fromstring(signal, np.int16)


signal = signal.reshape((spf.getnchannels(), -1))
print("Reshaping stereo file")

print(signal.shape)

plt.figure(1)
plt.title("Signal Wave...")
plt.plot(signal[0])
plt.show()

win = gaussian(64, std=1.0, sym=True)

SFT = ShortTimeFFT(win, hop=64, fs=44100, mfft=1024, scale_to="magnitude")

plt.imshow(SFT.spectrogram(signal[0]))
plt.show()