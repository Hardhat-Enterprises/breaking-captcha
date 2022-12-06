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


def invoke_predictor(path_to_data, host = '0.0.0.0', port = '8888'):
    import requests
    import json 
    import ast

    # host = '0.0.0.0'
    # port = '8888'

    url = f'http://{host}:{port}/invocations'
    headers = {
        'Content-Type': 'application/json',
    }

    testing_data = image_processor(path_to_data)
    testing_data = segmentation(testing_data)
    # 'samples/2b827.png'
    # test_data is a Pandas dataframe with data for testing the ML model
    http_data = json.dumps({'inputs': testing_data.tolist()})

    r = requests.post(url=url, headers=headers, data=http_data)

    pred = ast.literal_eval(r.text)['predictions']
    pred = np.argmax(pred, axis = 1)
    pred = [str(i) for i in pred]

    print(pred)

if __name__ == '__main__':

    invoke_predictor('samples/2b827.png')

