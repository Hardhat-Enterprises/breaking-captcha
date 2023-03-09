import cv2  as cv
import numpy as np 
import glob
from scipy import misc
import matplotlib.pyplot as plt


folder_dir = ' ' #storing the location of all the images in variable path


for images in glob.iglob(f'{folder_dir}/*'):
    
    pic = cv.imread(images)
    
    #convert to gray scale
    gray_pic = cv.cvtColor(pic, cv.COLOR_BGR2GRAY)
    
    # setting binary threshold
    ret,thresh1 = cv.threshold(gray_pic,175,255,cv.THRESH_BINARY)
    # inverting  - this will need to have a pixel test to make sure that each image is on a
    # white background with black text to make more readable
    ret,thresh2 = cv.threshold(gray_pic,140,255,cv.THRESH_BINARY_INV)
    
    
    titles = ['Original Image','Gray','BINARY','BINARY_INV']
    images = [pic, gray_pic, thresh1, thresh2]
    for i in range(4):
        plt.subplot(2,2,i+1),plt.imshow(images[i],'gray',vmin=0,vmax=255)
        plt.title(titles[i])
        plt.xticks([]),plt.yticks([])
        print(" ")
    plt.show()