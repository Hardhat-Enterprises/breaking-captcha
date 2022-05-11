import numpy as np
import cv2 as cv
import glob
from scipy import misc
import matplotlib.pyplot as plt
import pytesseract
import imutils

# change the directory to where you have the images stored
# if it's in an online repo, change to use urllib (import the library too)
folder_dir = '/Users/reneefarina/Desktop/SIT374-ProjectA/test_d/'

# iterate over files in
# that directory
for images in glob.iglob(f'{folder_dir}/*'):
    
    pic = cv.imread(images)
    
    # these are all the variables for each pre-processing module applied to all the images in the folder
    # the interger values can be changed, from the opencv library documentation: "The second argument is the threshold value 
    # which is used to classify the pixel values. 
    #The third argument is the maximum value which is assigned to pixel values exceeding the threshold"
    
    gray_pic = cv.cvtColor(pic, cv.COLOR_BGR2GRAY)
    ret,thresh1 = cv.threshold(gray_pic,175,255,cv.THRESH_BINARY)
    ret,thresh2 = cv.threshold(gray_pic,140,255,cv.THRESH_BINARY_INV)
    ret,thresh4 = cv.threshold(blur,100,190,cv.THRESH_BINARY)
    
    # The parameters are image, sigmaX and sigmaY, and border type
    blur = cv.GaussianBlur(gray_pic, (5, 5), 0)
    
    
    # more image processing and de-noising
    kernel = np.ones((2,2), np.uint8)
    dilation = cv.dilate(gray_pic, kernel, iterations = 1)
    dilation2 = cv.dilate(blur, kernel, iterations = 1)
    dilation3 = cv.dilate(thresh4, kernel, iterations = 1)
    
    erosion = cv.erode(dilation, kernel, iterations = 1)
    erosion2 = cv.erode(dilation2, kernel, iterations = 1)
    erosion3 = cv.erode(dilation3, kernel, iterations = 1)
    
    #cYou will need to check with the segmentation team about these values, as I believe they are arbitary - which 
    # may not work with each image - you can see from the 3rd image, the last box doesn't fully capture the "m"
    # this maybe intended, but we may have to fine tune for the next iteration of the project
    x, y, w, h = 31, 13, 21, 39
    
    #Titles and images to display in the plot - the titles may need adjusted depending on what pro-processing image
    #you want to plot
    titles = ['Original Image','Gray','BINARY','BINARY_INV', 'Blur', "Blur + threshold", "dilation1", "dilation2", "dilation3", "Gaussian + Otsu"]
    
    # this is not showing every single pre-processing variable, however, you can add more/take some away, you will
    # have to adjust the for loop if you change this
    images = [pic, gray_pic, thresh1, thresh2, blur, thresh4, dilation, dilation2, dilation3, erosion3]

    # i have added this so that the while loop that plots the lines around each image doesn't keep dupliacting and
    p = 1    
    
    #for all the items in the folder, iterate over each and display all the pre-processing included in the images variable:
    for i, item in enumerate(images):

        plt.subplot(5,2,i+1),plt.imshow(images[i],'gray',vmin=0,vmax=255)
        
       
        while p < 6:
            cv.rectangle(erosion3, (x, y), (x + w, y + h), (0, 255, 0), 2)
            x += w
            p+=1
        
        plt.title(titles[i])
        plt.xticks([]),plt.yticks([])
    plt.show()
