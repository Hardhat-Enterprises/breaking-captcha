import cv2 
import numpy as np 
import glob
from scipy import misc
import matplotlib.pyplot as plt


path = glob.glob("enter_folder_or_path_here") #storing the location of all the images in variable path
outpath = ("enter_folder_orpath_here")
#imgs = [] #creating an empty list

image_no = 1
for img in path: #running a loop to iterate through every image in the file
    pic = cv2.imread(img) #reading the image using matplotlib
    gray_pic = cv2.cvtColor(pic, cv2.COLOR_BGR2GRAY) #converting the image into grayscale
    r, threshold = cv2.threshold(gray_pic, 110, 255, cv2.THRESH_OTSU) #converting the image into grayscale using the histogram method
    
    # need add de-noise element from exploration python file.
    

    ## This will write out code to a new folder on the local host - may need remove
    cv2.imwrite(outpath + str(image_no) + '.jpg', threshold) # write to the test_pImg folder 
    image_no +=1 