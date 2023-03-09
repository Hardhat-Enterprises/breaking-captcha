# Install packages to your environment as necessary
import os
import numpy as np
import pandas as pd
from scipy.io import wavfile
from python_speech_features import mfcc

if __name__=='__main__':

#Save the input directory directory to input_folder
	input_folder = "./audio/"

	X = np.array([])

		# Iterate through all audio files and extract MFCCs 
	for filename in [x for x in os.listdir(input_folder) if x.endswith('.wav')]:
		# Read the input file
		filepath = os.path.join(input_folder, filename)
		sampling_freq, audio = wavfile.read(filepath)

		# Extract MFCC features. As per python_speech_features, features come in frames x numcep shape
		print("Extracting MFCC for " + filename + "...")

		mfcc_features = mfcc(audio, preemph=0.97,winlen=0.025, winstep=0.01, nfft=402, nfilt=26, appendEnergy=True, numcep=26, winfunc=np.hamming)

		X = mfcc_features
		df = pd.DataFrame(X)
		
	print(X)
	print(df)
	print(type(df))