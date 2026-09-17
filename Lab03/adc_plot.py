import matplotlib.pyplot as plt
import numpy as np
import csv

with open('data.csv', 'r') as file:
    reader = csv.reader(file)
    data = list(reader)
    t = [int(row[0]) for row in data]
    v = [int(row[1]) for row in data]

mean = np.mean(v)
std = np.std(v)

#noise graph
plt.plot(t, v)
plt.axhline(y=mean, color='r', linestyle='--', label=f'Mean: {mean:.2f}')
plt.title('ADC Noise Plot')
plt.xlabel('Time (us)')
plt.ylabel('Voltage (mV)')
plt.legend()
plt.savefig('adc_noise_plot.png')
plt.show()

#histogram
plt.hist(v, bins=20, edgecolor='black')
plt.axvline(x=mean, color='r', linestyle='--', label=f'Mean: {mean:.2f}')
plt.axvline(x=mean + std, color='g', linestyle='--', label=f'Mean + 1 Std: {mean + std:.2f}')
plt.axvline(x=mean - std, color='g', linestyle='--', label=f'Mean - 1 Std: {mean - std:.2f}')
plt.title('ADC Noise Histogram')
plt.xlabel('Voltage (mV)')
plt.ylabel('Frequency')
plt.legend()
plt.savefig('adc_noise_histogram.png')
plt.show()

