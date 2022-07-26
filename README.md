<<<<<<< HEAD
# Breaking CAPTCHA Project

Project lead: Andrew Hallam (ahallam@deakin.edu.au)

## Project Description
This project is about using a combination of Artificial Intelligence (AI), Machine Learning (ML), and Deep Learning (DL) technique to create algorithms that crack different CAPTCHA system

Deliverables:
- AI/ML Models that can crack CAPTCHAs of different forms
- Documentation showcasing the development of the model (documentation should also track discontinued models, so that they are not re-attempted)
- Hand-over Documentation
- (Long Term) - a software that interacts with browsers to detect a CAPTCHA and and crack it.

## Team Structure
**Data Engineering/Data Scrappings:**

Lead: Hugh Wan (wanjunyuan@deakin.edu.au)
- Github Responsibilities (maintaining, approving merge requests, etc.)
- Data Responsibilities (obtaining, integrity, maintaining)
- Data Pipeline
- Real time website scraping

**Project Team A (Image based CAPTCHA)**

Lead: William Tan Yoon Lok (tanyoonlok@deakin.edu.au)
- Research (both CAPTCHA and Model research)
- Model development
- Model Reviews

**Project Team B (Text based CAPTCHA)**

Lead: Cecilia Sammut (ccsammut@deakin.edu.au)
- Research (both CAPTCHA and Model research)
- Model developmentsss
- Model Review

## Resource
### Relevant links
I have compiled some of the findings based on all the post from MS Teams and can be seen below:</p>
- [Kaggle Captcha version 2 Image by fournierp](https://www.kaggle.com/fournierp/captcha-version-2-images)
- [Kaggle Cracking captcha by aakashnain](https://www.kaggle.com/aakashnain/cracking-captcha)
- [Learning resource about Python, ML & AL by Angus](https://deakin365.sharepoint.com/sites/DeakinSITCapstone/Shared%20Documents/Forms/AllItems.aspx?id=%2Fsites%2FDeakinSITCapstone%2FShared%20Documents%2FDS%20and%20AI%20Community%2FResources%2FCheat%2Dsheets&p=true)

### Data Description
- [CAPTCHA breaking with Deep Learning](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwi9-Jby1db2AhVU7HMBHamQCb0QFnoECAUQAQ&url=http%3A%2F%2Fcs229.stanford.edu%2Fproj2017%2Ffinal-reports%2F5239112.pdf&usg=AOvVaw1COvEZdWx9HChGeN4bhUJh): This research paper mention that the have used *PyCaptcha*, which is a python package for CAPTCHA generation, to make custom CAPTCHA image dataset. It offers several degree of freedom such as font style, distortion and noise, so that people can exploit increase the diversity of the data and the difficulty of the recognition task.

## Operating GitHub (Pull requests) 
### Why GitHub? 
GitHub is a version control system that allows for collaboration amongst team members working on a project. It is also publishing a platform that allows all members to view the changes that have been made to strcture of the project or the code. It enables us to keep track of the codebase, save our projects as it develop and revisit prior points of the project should it be required.

### Collaborating Through GitHub
You can use GitHub either via its web version or desktop application. Branches are the central operating mechanism GitHub uses for collaboration. It allows us to have different versions of a repository simultaneously without making an change to the main source of code. The work done on different branches will not show up on the main branch until you merge it, allowing for experimentation with the code.

### Pull requests
On the web version of GitHub, you simply have to select the branch you want to work on, be in the 'main' or other branches after which, you will select the edit icon. After adding or making any changes to the code or documentation, scroll down to bottom and select 'Create a new branch for this commit and start a pull request'. Rename the branch any way you'd like and click on 'propose changes'. The following page provides you with an option to 'leave a comment' and you should comment on the exact nature of your changes. This will help other members understand your changes more efficiently. Once done, click 'Create pull request'. It then brings you to a page where you 'push' your changes to the selected branch. Again, it provides you with an option to 'leave a comment' and once you have explained the changes made, select 'Merge pull request' and then select 'Confirm merge'. This step will merge all the changes with the main branch that was selected. Finally, select 'delete branch', which deletes the branch that was copied and you were working on prior to the merge. Go back to the main page to view your changes. 

### Video guide on operating GitHub 
For a step-by-step guide on how to operate GitHub, you can click on this link: https://www.youtube.com/watch?v=RGOj5yH7evk. The link provides a good foundation on understanding not only GitHub but also Git.

### GitHub Admin
You can contact me if you have any questions regarding this repository:
- Hugh (wanjunyuan@deakin.edu.au)
- Rayvinder Athwal (rayv@deakin.edu.au)

This documentation will be updated constantly so keep an eye out!!

Last edited: 21/03/2022 6:36PM
=======
# NotARobot - Automating reCAPTCHAs with Tensorflow
######
This project leverages the Tensorflow Object Detection API to automatically solve Google reCAPTCHAs.

The procedure is as follows:

1. Locate and click the reCAPTCHA checkbox.
2. Locate the reCAPTCHA text and image(s).
3. Read the text using OCR.
4. Load the appropriate model based on the text.
5. Denoise the image using Nvidia's Noise2Noise implementation.
6. Classify the reCaptcha based on type (3x3 grid, 4x4 grid, 2x4 grid).
7. Detect objects within the captcha.
8. Click the appropriate boxes and verify.
9. Profit?

## Details
The detection of objects within the reCAPTCHA is accomplished using Google's Faster-RCNN NASNet architecture. Images were gathered from the Open Images V4 Dataset, and trained on a Titan RTX using a batch size of 1. Currently, 5 of the most common types have been trained, these being cars, buses, bicycles, fire hydrants, and traffic lights. Depending on availability from Open Images, each class included between 400 and 17000 images of training and validation data.

The detection of checkboxes and the reCAPTCHA itself uses the SSD Mobilenet architecture, as this lighweight model is better suited for the task. These models were trained using approximately 100 screenshots that I created.

The classification of reCaptchas is done with a simple Tensorflow classification model. This was trained on about 300 screenshots, and is relatively proficient at separating 3x3, 4x4, and 2x4 captchas. This is important as they determine what coordinates should be used for each square of the captcha.

The denoising algorithm uses Nvidia's noise2noise library. It is trained using Gaussian noise on a dataset of 15,000 reCaptcha images that I collected. This library is interesting in that data does not have to be labelled, allowing for easy use of large datasets. Denoising is beneficial in reCaptcha solving, as Google has begun implementing adversarial noise in a portion of images.

## Examples

reCAPTCHA         |  Detections
:-------------------------:|:-------------------------:
![](https://github.com/Possums/NotARobot/raw/master/img/captcha.png)  |  ![](https://github.com/Possums/NotARobot/raw/master/img/detections.png)

### Video demo
[![Video demo](http://img.youtube.com/vi/_OS5xOHxUtU/0.jpg)](http://www.youtube.com/watch?v=_OS5xOHxUtU "Video demo")

## Getting Started
### Hardware
A GPU capable of inference on the NASNet architecture is required (approximately 8GB of VRAM or more).

### Python dependencies
This project was run on Ubuntu 19.04 running Python3.7. Dependencies include pyautogui, pillow, numpy, matplotlib, and tensorflow. By default, it opens the Chrome browser, but this is easy to change according to preference. These can be installed by running the following:
```
pip3 install -r requirements.txt
```
Note: I recommend either compiling Tensorflow yourself, or installing the tensorflow-gpu package for best results.

### Models
Each model used is approximately 1.2 GB, and thus cannot be included in Github repositories. They are hosted in Google Drive at the following links:

|Class |	URL |
|-------------|-------------|
|Bicycle |	[link](https://drive.google.com/file/d/19dSW-_TfIY03s-0xjwmqQrlkjXy0dzcr/view?usp=sharing) |
|Bus | [link](https://drive.google.com/file/d/1fGFZpI3IsVIhW4bKc7_-UQjkHmYg_knv/view?usp=sharing) |
|Captcha image(s) |	[link](https://drive.google.com/file/d/1N0yMl2f5nT1eFTZvK6QpHQ33uexMeayM/view?usp=sharing) |
|Car	| [link](https://drive.google.com/file/d/1qUA0PRJmtNINpS7bdpT0wur19Fd1EKLN/view?usp=sharing) |
|Captcha checkbox	| [link](https://drive.google.com/file/d/11MIzTNSrGRU66Qws-EH0WfXEGVFssCCz/view?usp=sharing) |
|Fire Hydrant	| [link](https://drive.google.com/file/d/1pYbTFR2_XseQ937Yoeih93ediyZnifbu/view?usp=sharing) |
|Traffic Light	| [link](https://drive.google.com/file/d/1GC2LTI2U_nNlX08__HQ97V2QjgO00_Ey/view?usp=sharing) |

Each model is compressed into a tar.gz, and should be extracted into the object_detection directory.

### Usage

Simply run

```
export PYTHONPATH=$PYTHONPATH:`pwd`:`pwd`/slim
python3 run.py
```

### Improving your model:
There are a number of ways for you to improve your model. It can come in the form of adjusting the learning rate of your model, adding regularization features such as batch normalization or dropout and even activation functions. In addition, your model can be tested against newly generated data via data augmentation. Data Augmentation can be utilizied via Keras's ImageDataGenerator library. Examples of these techniques can be viewed through this link: https://www.tensorflow.org/tutorials/images/data_augmentation. 


### Additional data:
- [The CIFAR-10 dataset](https://www.cs.toronto.edu/~kriz/cifar.html): It consist of 60000 32x32 colour images in 10 classes, with 6000 image per class. There are 50000 training images and 10000 test images.
- [reCAPTCHA dataset](https://github.com/brian-the-dev/recaptcha-dataset): It contains thousands of image dataset
- [Various images from COCO](https://cocodataset.org/#download)
- [Image recognition using Tensorflow](https://github.com/alielabridi/Image-recognition-reCaptcha-TensorFlow): Image recognition using Machine Learning, Tensorflow and Google Inception Neural Networks to fool the Recaptcha System for Recognition of Bots. Demonstration

### Relevant research paper
- [An Object Detection based Solver for Google’s Image reCAPTCHA v2
](https://www.usenix.org/system/files/raid20-hossen.pdf)

>>>>>>> 6c2a8fcf7eb93d074a2e9edbe95665afb7279460
