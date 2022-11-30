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


