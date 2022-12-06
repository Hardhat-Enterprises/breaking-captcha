# coding=utf-8
from flask import Flask, render_template, request
import os
import numpy as np
import pandas as pd
from tensorflow import keras as k
import cv2
from PIL import Image
from tensorflow.keras.preprocessing.image import img_to_array
from sklearn.preprocessing import LabelEncoder, OneHotEncoder
from sklearn.model_selection import train_test_split
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Activation, MaxPooling2D, Flatten, Conv2D, Dropout, Dense
import mlflow
import sys

# ################################################################################
# ------------------------------- Text Prediction -------------------------------
# ################################################################################

# Image processor
def image_processor(image_path):
    path = image_path
        
    image = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
    image = cv2.adaptiveThreshold(image, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 145, 0)

    kernel = np.ones((5,5),np.uint8)
    image = cv2.morphologyEx(image, cv2.MORPH_CLOSE, kernel)
    
    kernel = np.ones((2,2),np.uint8)
    image = cv2.dilate(image, kernel, iterations = 1)

    image = cv2.GaussianBlur(image, (5,5), 0)

    return image

# Image segmentation
def segmentation(image):

    # naive segmentation
    x = [image[10:50,30:50],image[10:50,50:70],
                image[10:50,70:90],image[10:50,90:110],image[10:50,110:130]]
    # for each segment process           
    X = []
    for i in range(5) :
      X.append(img_to_array(Image.fromarray(x[i])))

    X = np.array(X)
    X = X.astype('float32')
    X /= 255.0

    return X

# Build model
def build_model(categories,row, col):
  # Build model
  model = Sequential()

  model.add(Conv2D(filters=16,kernel_size=(3,3), padding='same', input_shape=(row, col,1)))
  model.add(Activation('relu'))
  model.add(MaxPooling2D(pool_size = (2,2)))

  model.add(Conv2D(filters=16,kernel_size=(3,3), padding='same'))
  model.add(Activation('relu'))
  model.add(MaxPooling2D(pool_size = (2,2)))

  model.add(Conv2D(filters=32,kernel_size=(3,3), padding='same'))
  model.add(Activation('relu'))
  model.add(MaxPooling2D(pool_size = (2,2)))

  model.add(Conv2D(filters=32,kernel_size=(3,3), padding='same'))
  model.add(Activation('relu'))
  model.add(MaxPooling2D(pool_size = (2,2)))


  model.add(Flatten())
  model.add(Dropout(0.4))
  model.add(Dense(1500))
  model.add(Activation('relu'))
  model.add(Dropout(0.2))
  model.add(Dense(categories))
  model.add(Activation("softmax"))

  model.compile(loss = 'categorical_crossentropy',
                optimizer = 'adam' ,
                metrics = ['accuracy'])

  print(model.summary())

  return model

# Trainer
def trainer(sample_path, batch_size = 150, epochs = 40):

#   mlflow.set_tracking_uri("http://localhost:8000")
#   mlflow.set_experiment("text-captcha0")

  # Since there are subfolders inside the input directory, we've used nested loops
	X = []
	y = []
	for i, pic in enumerate(os.listdir(sample_path)):
		path = os.path.join(sample_path, pic)
		# Preprocess  
		if path[-3:] != 'png':
			# print('skipped')
			continue

		processed_image = image_processor(path)
		# Segmentation
		segmented_array = segmentation(processed_image)

		# Construct and Label
		for i in range(5):
			X.append(segmented_array[i])
			y.append(path[len(path)-9:len(path)-4][i])
		
	X = np.array(X)
	y = np.array(y)

	# Labeling Processing
	y_le = LabelEncoder().fit_transform(y)
	y_ohe = OneHotEncoder(sparse = False).fit_transform(y_le.reshape(len(y_le),1))

	# Train Test Split
	X_train, X_test, y_train, y_test = train_test_split(X, y_ohe, test_size = 0.2, random_state = 42)

	row, col = X.shape[1], X.shape[2]
	categories = y_ohe.shape[1]

	info = {y_le[i] : y[i] for i in range(len(y))}

	# Build Model
	model = build_model(categories, row, col)

	# Train and Validate
	model.fit(X_train, y_train,
			batch_size=batch_size,
			epochs=epochs,
			validation_data=(X_test, y_test),
			shuffle=True)

	return model, info

def text_predictor(img_path, trained_model, info):
  x = image_processor(img_path)
  X_pred = segmentation(x)

  y_pred = trained_model.predict(X_pred)
  y_pred = np.argmax(y_pred, axis = 1)

  output = []
  for res in y_pred:
    output.append(info[res])
  return (''.join(output))

# ################################################################################
# ------------------------------- End Text Prediction -----------------------------
# ################################################################################
app = Flask(__name__)

trained_model, info = trainer('input/captcha-version-2-images/samples')

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

		p = text_predictor(img_path, trained_model, info)

	return render_template("index.html", prediction = p, img_path = img_path)

if __name__ =='__main__':
    app.run(debug=True, host="0.0.0.0", port=int(os.environ.get("PORT", 8080)))
