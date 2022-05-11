# Text CAPTCHA project for Breaking CAPTCHA project

By attempting to re-work the various stages of the Decaptcha model developed by Bursztein et al. (2011) we aim to make improvements to the successful text CAPTCHA break rates for various text CAPTCHAs. This work will contribute to the goal of Hardhat Enterprises by developing a cyber weapon that will hopefully encourage more proactive work into developing a CAPTCHA scheme that will offer more security (and ideally more usability) than those available today. 

## How are we doing it?

We have isolated each step fo the Decaptcha pipeline and are trialling various methods to build our own pipeline that we aim to have improved performance when breaking text CAPTCHAs.

### Preprocessing

We will first replicate Decaptcha by binarising our CAPTCHAs. Once we set the baseline we can experiement with different types of image processing.

### Segmentation

Wea re trialling two different methods:

##### 1. Segmentation and recognition 

We are experimenting with methods that will allow us to perform segmentation and recognition simultaneously.

##### 2. Non-Segmentation

We are entering this somewhat theoretical field of non-segmentation recognition

### Post-Segmentation

This step is contingent on the outcomes of the steps above. It is necessary only if we need to feed data through for recognition.

### Recognition

We will experiment with different parameters to achieve optimal results for a SVM classifier. We will also experiment with other types of classification models to see if we can make some improvements on this step of the pipeline.

### Post-processing

We may not need this step.

## A text image data for captchat
- [Captcha Solving Using TensorFlow](https://github.com/JackonYang/captcha-tensorflow):Image Captcha Solving Using TensorFlow and CNN Model.
- [CAPTCHA using Deep learning](https://github.com/Vykstorm/CaptchaDL)
- [Breaking captchas with deep learning, Keras, and TensorFlow](https://pyimagesearch.com/2021/07/14/breaking-captchas-with-deep-learning-keras-and-tensorflow/)
- [Using deep learning to automatically break CAPTCHAs](https://github.com/tharidu/breakingcaptcha/blob/master/report/CAPTCHA-report.md)
- [Captcha Images](https://www.kaggle.com/datasets/aadhavvignesh/captcha-images): Captcha Images with different styles, fonts, colors

## Improving your model:
There are a number of ways for you to improve your model. It can come in the form of adjusting the learning rate of your model, adding regularization features such as batch normalization or dropout and even activation functions. In addition, your model can be tested against newly generated data via data augmentation. Data Augmentation can be utilizied via Keras's ImageDataGenerator library. Examples of these techniques can be viewed through this link: https://www.tensorflow.org/tutorials/images/data_augmentation.

## Relevant paper
- [CAPTCHA Recognition Using Deep Learning with Attached Binary Images](https://www.mdpi.com/2079-9292/9/9/1522/htm#sec3dot2-electronics-09-01522): 
- BURSZTEIN, E., AIGRAIN, J., MOSCICKI, A. & MITCHELL, J. C. The End is Nigh: Generic Solving of Text-based CAPTCHAs.  WOOT, 2014. 
- BURSZTEIN, E., MARTIN, M. & MITCHELL, J. 2011. Text-based CAPTCHA strengths and weaknesses. Proceedings of the 18th ACM conference on Computer and communications security. Chicago, Illinois, USA: Association for Computing Machinery. 
