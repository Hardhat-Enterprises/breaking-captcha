# Data Collection Automation Tool

This is the project directory for Data Collection Automation Tool for 
Breaking Captcha team.

Contributors:

* Fenglu Cai (caife@deakin.edu.au)

## Executive Summary

The aim of this tool is to eliminate the need to manually download CAPTCHAs
as dataset, which significantly improves productivity.

The approach taken is using Selenium to mimic human mouse and keyboard actions.


## Functions:

1. Support easy download of audio and image CAPTCHAs from Botdetect website:
https://captcha.com/demos/features/captcha-demo.aspx
2. Support easy download of audio CAPTCHAs from google reCAPTCHA website:
https://recaptcha-demo.appspot.com/recaptcha-v2-checkbox-explicit.php

## Usage:

from data_collection.google_recapture_helper import *
from data_collection.botdetect_captcha_helper import *


#download 100 images (file names: 0.jpg-100.jpg) and 100 audio (file names: 0.wav-100.wav) CAPTCHAs from botdetect:
 
task = GoogleTask()
task.load_webpage()
task.do(0,100)

#download 100 audio CAPTCHAs (file names: 0.mp3-100.mp3) from google:
task = GoogleTask()
task.load_webpage()
task.do(0,100)

## The default setting is to download CAPTCHAs of length 1
How to change desired CAPTCHA length?
In BotdetectTask::load_webpage(), change select.select_by_visible_text(‘1’) to the desired length
 you can find BotdetectTask class in data_collection/botdetect_captcha_helper.py,


## Download range 
If the program has to be stopped when it is running, when you restart the programme, please indicate the next appropriate range in task.do(from, to).
This is because the number of files to download is controlled by task.do(from, to) 
note: to has to be larger than from because it represents the naming convention in increasing order for a range of files 
for example, task.do(1,3) instructs the following 3 pairs of  CAPTCHA files to be downloaded:
1.jpg, 1.wav,  2.jpg, 2.wav,  3.jpg, 3.wav
Therefore, we can restart downloading task simply by designating the next desirable range


## Automation speed acceleration.
In essence, this tool was designed to mimic human-being click the mouse and hit the keyboard, by which I mean any such action follows a random sleep time like humans have. Since botdetect website seems to have no detection for automating tools, time interval between actions can be very short. The sleep time between each action observes a uniform distribution.


How to change it?

Sleep_decorator_short() is for speed control of the botdetect CAPTCHA download automation.
You can find the function in data_collection/__init__.py. 
Note: data_collection/google_recaptcha_helper.py is to download audio CAPTCHAs from google reCAPTCHA v2 website. The reason why it was not put to use is because the google website detects automated queries. However, the code can be used for further reference for the following reasons:


1. How to extend to include another source. 
Apart from the botdetect website, the google reCAPTCHA website demonstrates how to extend the automation tool to include more downloading resources. The tool uses Object Oriented Programming(OOP) for easier maintainability. Please refer to the class UML in github.


2. How to increase more randomness in this automation tool 
Unlike the botdetect downloading, which uses selenium to find webpage elements, it uses pyautogui which can indicate the coordinates on screen you want to click. This means that you are able to hit mouse in random coordinates around an interested area (when humans click the mouse, not only the time interval between actions are random, each time they click on the same element, they normally do not click on the same coordinate),  which could be helpful when the automation detection level of a particular source is not very high. 


