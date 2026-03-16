import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft, fftfreq

# 1. IEEE Publication Formatting Settings
plt.rcParams['font.family'] = 'serif'
plt.rcParams['font.serif'] = ['Times New Roman'] + plt.rcParams['font.serif']
plt.rcParams['font.size'] = 10
plt.rcParams['axes.labelsize'] = 10
plt.rcParams['axes.titlesize'] = 10
plt.rcParams['xtick.labelsize'] = 8
plt.rcParams['ytick.labelsize'] = 8
plt.rcParams['figure.dpi'] = 300 # 300 DPI is required for IEEE print quality

# 2. Load the Data
# Ensure your CSV is in the same folder and named correctly
df = pd.read_csv('anomaly_vibration.csv')

# Edge Impulse CSVs typically start data on the second row. 
# We will grab just the first 50 samples (1 second of data at 50Hz)
# Change 'accel_x' if your column is named differently (e.g., 'accX')
sample_rate = 50 # Hz
N = 50 # 1 second window
time = np.linspace(0, 1.0, N, endpoint=False)
signal = df['accel_x'].values[0:N]

# 3. Calculate the Fast Fourier Transform (FFT)
# This converts the time signal into frequency peaks
yf = fft(signal)
xf = fftfreq(N, 1 / sample_rate)[:N//2]
# Get the magnitude (amplitude) of the frequencies
amplitude = 2.0/N * np.abs(yf[0:N//2])

# 4. Create the Subplots (1 Column, 2 Rows)
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(3.5, 4.5)) # 3.5 inches fits exactly in one IEEE column

# --- Top Plot: Time Domain (Raw Wave) ---
ax1.plot(time, signal, color='#1f77b4', linewidth=1)
ax1.set_title('Time-Domain: Raw Acceleration ($A_x$)')
ax1.set_xlabel('Time (seconds)')
ax1.set_ylabel('Amplitude ($g$)')
ax1.grid(True, linestyle='--', alpha=0.6)

# --- Bottom Plot: Frequency Domain (FFT) ---
ax2.plot(xf, amplitude, color='#ff7f0e', linewidth=1.2)
ax2.set_title('Frequency-Domain: Spectral Peaks (FFT)')
ax2.set_xlabel('Frequency (Hz)')
ax2.set_ylabel('Magnitude')
ax2.grid(True, linestyle='--', alpha=0.6)

# 5. Final Layout Adjustments & Export
plt.tight_layout()
plt.savefig('figure4_dsp_fft.png', bbox_inches='tight', dpi=300)
plt.savefig('figure4_dsp_fft.pdf', bbox_inches='tight') # PDFs are best for LaTeX
plt.show()