# coding=utf-8
from flask import Flask, render_template, request

# ################################################################################
# ------------------------------- Text Prediction -------------------------------
# ################################################################################
import os
import numpy as np
import pandas as pd
import cv2
from PIL import Image
from tensorflow.keras.preprocessing.image import img_to_array
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC

X=[]
y=[]

# Since there are subfolders inside the input directory, we've used nested loops
for i, pic in enumerate(os.listdir('input/captcha-version-2-images/samples/samples')):
    path = os.path.join('input/captcha-version-2-images/samples/samples', pic)
        
    ### Preprocessing
    image = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
    image = cv2.adaptiveThreshold(image, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 145, 0)
    kernel = np.ones((5,5),np.uint8)
    image = cv2.morphologyEx(image, cv2.MORPH_CLOSE, kernel)
    kernel = np.ones((2,2),np.uint8)
    image = cv2.dilate(image, kernel, iterations = 1)
    image = cv2.GaussianBlur(image, (5,5), 0)

    ###Segmenting
    x=[image[10:50,30:50],image[10:50,50:70],
                image[10:50,70:90],image[10:50,90:110],image[10:50,110:130]]

    # Adding labels to the individual segmented letters
    for i in range(5):
        X.append(img_to_array(Image.fromarray(x[i])))
        y.append(path[len(path)-9:len(path)-4][i])

X=np.array(X)
y=np.array(y)

X = pd.DataFrame(X.reshape(X.shape[0],X.shape[1]*X.shape[2]*X.shape[3]))
X=X.astype('float32')
X/=255

#train_test split

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = 0.2, random_state = 42)

info = {y[i] : y[i] for i in range(len(y))}


# model - fit - predict
non_linear_model = SVC(kernel='rbf')
non_linear_model.fit(X_train, y_train)
y_pred = non_linear_model.predict(X_test)

### predictor function
def text_predictor(img_path) :
    
	img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)

	#preprocessing
	image = cv2.adaptiveThreshold(img, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 145, 0)
	image = cv2.adaptiveThreshold(image, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 145, 0)
	kernel = np.ones((5,5),np.uint8)
	image = cv2.morphologyEx(image, cv2.MORPH_CLOSE, kernel)
	kernel = np.ones((2,2),np.uint8)
	image = cv2.dilate(image, kernel, iterations = 1)
	image = cv2.GaussianBlur(image, (5,5), 0)

	#segmentation
	x = [image[10:50, 30:50], image[10:50, 50:70], image[10:50, 70:90],
					image[10:50, 90:110], image[10:50, 110:130]]

	X_pred = []
	for i in range(5) :
		X_pred.append(img_to_array(Image.fromarray(x[i])))

	X_pred = np.array(X_pred)
	X_pred/= 255.0
	X_pred = pd.DataFrame(X_pred.reshape(5,800))

	y_pred = non_linear_model.predict(X_pred)

	output = [info[res] for res in y_pred]
	return (''.join(output))

# ################################################################################
# ------------------------------- End Text Prediction -----------------------------
# ################################################################################

# ################################################################################
# ------------------------------- Start Audio Prediction -------------------------
# ################################################################################

# Audio analysis & ML
import torch
import torchaudio
from speechbrain.pretrained import SpectralMaskEnhancement
from transformers import Speech2TextProcessor, Speech2TextForConditionalGeneration
from transformers import pipeline
from transformers import Wav2Vec2Processor, Wav2Vec2ForCTC
from speechbrain.pretrained import WaveformEnhancement

#################
# Enhancement
#################
# Speech brain metricgan-plus-voicebank
def enhance_audio_metrican(path_to_file):
    # setup spectral enhancement from speechbrain
    enhance_model = SpectralMaskEnhancement.from_hparams("input/models/pretrained_models/metricgan-plus-voicebank")

    # Load and add fake batch dimension
    original_ = enhance_model.load_audio(path_to_file).unsqueeze(0)

    # Enhance
    enhanced_ = enhance_model.enhance_batch(original_, lengths=torch.tensor([1.]))

    return enhanced_ # This is 2 levels deep tensor [[s1, s2, ..., sn]]


# Speech brain metricgan-plus-voicebank
def enhance_audio_mimic(path_to_file):
    # setup spectral enhancement from speechbrain
    enhance_model = WaveformEnhancement.from_hparams("input/models/pretrained_models/mtl-mimic-voicebank")
    # enhanced = enhance_model.enhance_file(path_to_file)
    # Load and add fake batch dimension
    original_ = enhance_model.load_audio(path_to_file).unsqueeze(0)

    # Enhance
    enhanced_ = enhance_model.enhance_batch(original_, lengths=torch.tensor([1.]))

    return enhanced_ # This is 2 levels deep tensor [[s1, s2, ..., sn]]


#################
# ASR
#################
# s2t-small-librispeech-asr
def transcribe_s2t(audio_sample, sample_rate=16000):
    # Instantiate model
    model = Speech2TextForConditionalGeneration.from_pretrained("input/models/pretrained_models/facebook/model/s2t-small-librispeech-asr")
    # Processor = Featurise + tokenise
    processor = Speech2TextProcessor.from_pretrained("input/models/pretrained_models/facebook/processor/s2t-small-librispeech-asr")
    inputs = processor(audio_sample, sampling_rate=sample_rate, return_tensors="pt")
    generated_ids = model.generate(inputs["input_features"], attention_mask=inputs["attention_mask"])
    # Transcribe
    transcription = processor.batch_decode(generated_ids)
    
    return [s.lower() for s in transcription]

# s2t-small-librispeech-asr
def transcribe_wave2vec(audio_sample, sample_rate=16000):
    # load model and tokenizer
    processor = Wav2Vec2Processor.from_pretrained("input/models/pretrained_models/facebook/processor/wav2vec2-base-960h")
    model = Wav2Vec2ForCTC.from_pretrained("input/models/pretrained_models/facebook/model/wav2vec2-base-960h")

    # tokenize
    input_values = processor(audio_sample, sampling_rate=sample_rate, return_tensors="pt", padding="longest").input_values  # Batch size 1

    # retrieve logits
    logits = model(input_values).logits

    # take argmax and decode
    predicted_ids = torch.argmax(logits, dim=-1)
    transcription = processor.batch_decode(predicted_ids)
    return [s.lower() for s in transcription]

#################
# Solver
#################


def solve_captcha(path_to_file, enhancement_functions, asr_functions):
    """ 
    Description:
    This function takes an input audio file path, a list of enhancement functions and a list of asr functions and returns a string of most common 
    that corresponds to the most common solution of testing each enhancement and asr combination
    """

    transcriptions = []
    for enhance in enhancement_functions:
        enhanced_audio = enhance(path_to_file)
        for asr in asr_functions:
            transcription = asr(enhanced_audio[0])
            transcriptions.append(transcription)
    
    # Extract most common if competing this will take the first
    values, counts = np.unique(transcriptions, return_counts=True)
    ind = np.argmax(counts)
    most_common = values[ind]

    return most_common#, transcriptions

def audio_predictor(path):
    # Audio analysis & ML
    return solve_captcha(path, [enhance_audio_metrican, enhance_audio_mimic], [transcribe_s2t, transcribe_wave2vec])

# ################################################################################
# ------------------------------- End Audio Prediction ---------------------------
# ################################################################################

app = Flask(__name__)

# routes
@app.route("/", methods=['GET', 'POST'])
def main():
    return render_template("index.html")

@app.route("/submit_image", methods = ['GET', 'POST'])
def get_text_output():
	if request.method == 'POST':
		img = request.files['image_file']

		img_path = "static/user_uploads/" + img.filename 
		img.save(img_path)

		p = text_predictor(img_path)

	return render_template("index.html", prediction = p, img_path = img_path)

@app.route("/submit_audio", methods = ['GET', 'POST'])
def get_audio_output():
	if request.method == 'POST':
		audio_file = request.files['audio_file']

		audio_file_path = "static/user_uploads/" + audio_file.filename
		audio_file.save(audio_file_path)

		p = audio_predictor(audio_file_path)

	return render_template("index.html", prediction = p, img_path = '/static/audio_file.png')

if __name__ =='__main__':
    app.run(debug=True, host="0.0.0.0", port=int(os.environ.get("PORT", 8080)))
