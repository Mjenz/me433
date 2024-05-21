from __future__ import print_function
from __future__ import division

import csv
import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

t = [] # column 0
data1 = [] # column 1
avg = [] 

def sample_rate(t):
    n = len(t) - 1 
    fs = n / (t[-1] - t[0]) 
    return fs

def finite_impluse_respone(t, data, cutoff, length,c):
    # Example code, computes the coefficients of a low-pass windowed-sinc filter.
    # Configuration.
    
    fS = int(len(data)/t[-1])  # Sampling rate.
    fL = cutoff[c]  # Cutoff frequency.
    N = length[c]  # Filter length, must be odd.
    print(fS)
    print(c)
    c = c+1
    # Compute sinc filter.
    h = np.sinc(2 * fL / fS * (np.arange(N) - (N - 1) / 2))

    # Apply window.
    h *= np.blackman(N)

    # Normalize to get unity gain.
    h /= np.sum(h)

    # print(h)

    # Applying the filter to a signal s can be as simple as writing
    avg = np.convolve(data, h, mode='valid')

    # # # Calculate both ffts
    y1, y2 = avg, data # the data to make the fft from
    n1, n2 = len(y1), len(y2) # length of the signal
    k1, k2 = np.arange(n1), np.arange(n2)
    Fs1, Fs2 = int(len(y1)/t[0:len(t)-N+1][-1]), int(len(y2)/t[-1]) # calculate sample rate
    T1, T2 = n1/Fs1, n2/Fs2
    frq1, frq2 = k1/T1, k2/T2 # two sides frequency range
    frq1, frq2 = frq1[range(int(n1/2))],frq2[range(int(n2/2))] # one side frequency range
    Y1, Y2 = np.fft.fft(y1)/n1, np.fft.fft(y2)/n2 # fft computing and normalization
    Y1, Y2 = Y1[range(int(n1/2))], Y2[range(int(n2/2))]

    # # # PLOT FFT

    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.title.set_text('Data vs. Filtered Data with FIR fL = '+ str(fL) + ' N = ' + str(N))
    ax1.plot(t,y2,'k')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.title.set_text('FFT Data vs. FFT Filtered Data with FIR fL = '+ str(fL) + ' N = ' + str(N))
    ax2.loglog(frq2,abs(Y2),'k') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y2(freq)|')

    ax1.plot(t[0:len(t)-N+1],y1,'r')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.loglog(frq1,abs(Y1),'r') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y1(freq)|')
    plt.show()

def infinite_impulse_response(t, data, A, B):
    avg = [0] * len(t)  # Initialize avg with the correct size
    for i in range(len(t)):
        if i == 0:
            avg[i] = A * 0 + B * data[i]

        avg[i] = A * avg[i-1] + B * data[i]

    # # # Calculate both ffts
    y1, y2 = avg, data # the data to make the fft from
    n1, n2 = len(y1), len(y2) # length of the signal
    k1, k2 = np.arange(n1), np.arange(n2)
    Fs1, Fs2 = int(len(y1)/t[-1]), int(len(y2)/t[-1]) # calculate sample rate
    T1, T2 = n1/Fs1, n2/Fs2
    frq1, frq2 = k1/T1, k2/T2 # two sides frequency range
    frq1, frq2 = frq1[range(int(n1/2))],frq2[range(int(n2/2))] # one side frequency range
    Y1, Y2 = np.fft.fft(y1)/n1, np.fft.fft(y2)/n2 # fft computing and normalization
    Y1, Y2 = Y1[range(int(n1/2))], Y2[range(int(n2/2))]

    # # # PLOT FFT

    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.title.set_text('Data vs. Filtered Data with IIR A = ' + str(A) + ' B = ' + str(B))
    ax1.plot(t,y2,'k')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.title.set_text('FFT Data vs. FFT Filtered Data with IIR A = ' + str(A) + ' B = ' + str(B))
    ax2.loglog(frq2,abs(Y2),'k') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y2(freq)|')

    ax1.plot(t,y1,'r')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.loglog(frq1,abs(Y1),'r') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y1(freq)|')
    plt.show()
        

def moving_average(t, data,x):
    # avg = [0] * (len(t) - x + 1)
    # time = [0] * (len(t) - x + 1)
    avg = []
    time = []
    temp = []
    # size = int(len(data)/t[-1])
    print('size ' + str(x))
    mask = np.ones((x)) / x
    # Use 'same' mode to ensure the output length matches the input length'
    time = np.convolve(t, mask, mode='valid')[::x]
    avg = np.convolve(data, mask, mode='valid')[::x]
    
    # # # Calculate both ffts
    y1, y2 = avg, data # the data to make the fft from
    n1, n2 = len(y1), len(y2) # length of the signal
    k1, k2 = np.arange(n1), np.arange(n2)
    Fs1, Fs2 = int(len(y1)/time[-1]), int(len(y2)/t[-1]) # calculate sample rate
    T1, T2 = n1/Fs1, n2/Fs2
    frq1, frq2 = k1/T1, k2/T2 # two sides frequency range
    frq1, frq2 = frq1[range(int(n1/2))],frq2[range(int(n2/2))] # one side frequency range
    Y1, Y2 = np.fft.fft(y1)/n1, np.fft.fft(y2)/n2 # fft computing and normalization
    Y1, Y2 = Y1[range(int(n1/2))], Y2[range(int(n2/2))]

    # # # PLOT FFT

    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.title.set_text('Data vs. Filtered Data with Moving Avg x = ' + str(x))
    ax1.plot(t,y2,'k')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.title.set_text('FFT Data vs. FFT Filtered Data with Moving Avg x = ' + str(x))
    ax2.loglog(frq2,abs(Y2),'k') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y2(freq)|')

    ax1.plot(time,y1,'r')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.loglog(frq1,abs(Y1),'r') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y1(freq)|')
    plt.show()
 
    

list = ['sigA.csv','sigB.csv','sigC.csv','sigD.csv']        

for i in range(len(list)):
    # open the csv

    # clear t and data1 for plotting
    t = []
    data1 = []
    with open(list[i]) as f:
        # open the csv file
        reader = csv.reader(f)
        for row in reader:
            # read the rows 1 one by one
            t.append(float(row[0])) # leftmost column
            data1.append(float(row[1])) # second column


    # moving_average(t,data1,5)
    # infinite_impulse_response(t, data1, .95, .05)
    finite_impluse_respone(t, data1, cutoff=[100,50,5000,15], length=[40,60,1,250],c=i)