# coding=utf-8
from flask import Flask, render_template, request

# ################################################################################
# ------------------------------- Text Prediction -------------------------------
# ################################################################################
# import os
# import numpy as np
# import pandas as pd
# import cv2
# from PIL import Image
from tensorflow.keras.preprocessing.image import img_to_array
# from sklearn.model_selection import train_test_split
# from sklearn.svm import SVC

# X=[]
# y=[]

# # Since there are subfolders inside the input directory, we've used nested loops
# for i, pic in enumerate(os.listdir('input/captcha-version-2-images/samples/samples')):
#     path = os.path.join('input/captcha-version-2-images/samples/samples', pic)
        
#     ### Preprocessing
#     image = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
#     image = cv2.adaptiveThreshold(image, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 145, 0)
#     kernel = np.ones((5,5),np.uint8)
#     image = cv2.morphologyEx(image, cv2.MORPH_CLOSE, kernel)
#     kernel = np.ones((2,2),np.uint8)
#     image = cv2.dilate(image, kernel, iterations = 1)
#     image = cv2.GaussianBlur(image, (5,5), 0)

#     ###Segmenting
#     x=[image[10:50,30:50],image[10:50,50:70],
#                 image[10:50,70:90],image[10:50,90:110],image[10:50,110:130]]

#     # Adding labels to the individual segmented letters
#     for i in range(5):
#         X.append(img_to_array(Image.fromarray(x[i])))
#         y.append(path[len(path)-9:len(path)-4][i])

# X=np.array(X)
# y=np.array(y)

# X = pd.DataFrame(X.reshape(X.shape[0],X.shape[1]*X.shape[2]*X.shape[3]))
# X=X.astype('float32')
# X/=255

# #train_test split

# X_train, X_test, y_train, y_test = train_test_split(X, y, test_size = 0.2, random_state = 42)

# info = {y[i] : y[i] for i in range(len(y))}


# # model - fit - predict
# non_linear_model = SVC(kernel='rbf')
# non_linear_model.fit(X_train, y_train)
# y_pred = non_linear_model.predict(X_test)

# ### predictor function
# def text_predictor(img_path) :
    
# 	img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)

# 	#preprocessing
# 	image = cv2.adaptiveThreshold(img, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 145, 0)
# 	image = cv2.adaptiveThreshold(image, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 145, 0)
# 	kernel = np.ones((5,5),np.uint8)
# 	image = cv2.morphologyEx(image, cv2.MORPH_CLOSE, kernel)
# 	kernel = np.ones((2,2),np.uint8)
# 	image = cv2.dilate(image, kernel, iterations = 1)
# 	image = cv2.GaussianBlur(image, (5,5), 0)

# 	#segmentation
# 	x = [image[10:50, 30:50], image[10:50, 50:70], image[10:50, 70:90],
# 					image[10:50, 90:110], image[10:50, 110:130]]

# 	X_pred = []
# 	for i in range(5) :
# 		X_pred.append(img_to_array(Image.fromarray(x[i])))

# 	X_pred = np.array(X_pred)
# 	X_pred/= 255.0
# 	X_pred = pd.DataFrame(X_pred.reshape(5,800))

# 	y_pred = non_linear_model.predict(X_pred)

# 	output = [info[res] for res in y_pred]
# 	return (''.join(output))

# ################################################################################
# ------------------------------- End Text Prediction -----------------------------
# ################################################################################

# app = Flask(__name__)

# # routes
# @app.route("/", methods=['GET', 'POST'])
# def main():
#     return render_template("index.html")

# @app.route("/submit_image", methods = ['GET', 'POST'])
# def get_text_output():
# 	if request.method == 'POST':
# 		img = request.files['image_file']

# 		img_path = "static/user_uploads/" + img.filename 
# 		img.save(img_path)

# 		p = text_predictor(img_path)

# 	return render_template("index.html", prediction = p, img_path = img_path)

# if __name__ =='__main__':
#     app.run(debug=True, host="0.0.0.0", port=int(os.environ.get("PORT", 8080)))
