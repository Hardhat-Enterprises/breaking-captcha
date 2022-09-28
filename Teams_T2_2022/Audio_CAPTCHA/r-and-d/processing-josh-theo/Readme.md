# Librosa Audio Pre-Processing
## _Joshua Tolding_

This project was made using the python Librosa Library for extracting Mels features from a
sample audio file. This code also displays a spectrogram for easier visualization.

## Features

- Import sample audio files
- Display audio features as a collection of numerical data stored in a numpy array
- Create two different spectrograms; one is an uncleaned audio and the other only displays melodic audio
- Store extracted melodic featurs in an array

## Libraries used
- Pandas - Handling series
- Numpy - used to store data in a numpy array
- Matplotlib - used for plotting
- Librosa - used for the audio pre-processing
- Glob - used for accessing files inside a folder
- OS - used to set working directory

## Project Walkthrough
##### Import necessary libraries
```sh
import pandas as pd
import numpy as np
import matplotlib.pylab as plt

from glob import glob

import librosa
import librosa.display
```

##### Set the working directory of the project; In this case it's where the audio files are located

```s
import os
os.chdir(audio source)
```

##### Use glob to access all audio files that ends with .wav and assign it to the variable audioFiles

```s
audioFiles = glob("audio/*.wav")
```

##### Load an audio file using the Librosa load function and store the features to the variable y and the sample rate to the variable sr

```s
y, sr = librosa.load(audioFiles[0])
```

##### Trim the given audio track by increasing or decreasing the max decibels; in this program it is set to 10db. After trimming plot it to check the difference.

```s
y_trimmed, index = librosa.effects.trim(y, top_db=10)
pd.Series(y_trimmed).plot(figsize = (10,5), lw = 1)
plt.show()
```

##### Transform the y values using the stft module of librosa which will then be used to get the absolute value of the converted data from amps to db

```s
D = librosa.stft(y)
S_db = librosa.amplitude_to_db(abs(D), ref=np.max)
S_db.shape
```

##### Create a spectrogram using the converted data

```s
fig, ax = plt.subplots(figsize = (10,5))
img = librosa.display.specshow(S_db,
                              x_axis = 'time',
                              y_axis = 'log',
                              ax = ax)
fig.colorbar(img, ax=ax, format=f"%0.2f")
plt.show()
```

##### Transform the stf data into a mel frequency by using adding n_mels to the parameter; this parameter will dictate how many mels will be shown

```s
S = librosa.feature.melspectrogram(y=y, sr=sr, n_mels=128)
S_db_mel = librosa.amplitude_to_db(S, ref=np.max)
```

##### Using the newly transformed data we will now create a mels spectrogram and we can see that there are fewer points in the visualization.

```s
fig, ax = plt.subplots(figsize = (10,5))
img = librosa.display.specshow(S_db_mel,
                              x_axis = 'time',
                              y_axis = 'log',
                              ax = ax)
fig.colorbar(img, ax=ax, format=f"%0.2f")
plt.show()
```

##### After creating a mels spectrogram the data can now be stored in an array and can now be fed to a model