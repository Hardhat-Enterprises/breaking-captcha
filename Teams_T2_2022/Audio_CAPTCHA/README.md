# Breaking Audio Captcha
This is the Project directory for Breaking Audio Captcha project for Deakin Capstone.

Contributers:

- Luka Croote (lcroote@deakin.edu.au)
- Ridwan Ganiyu (rbganiyu@deakin.edu.au)
- Theodore Hrysomallis (thrysomallis@deakin.edu.au)
- Josh Tolding (jtolding@deakin.edu.au)

## Executive Summary
The aim of this stream is to develop a program for breaking audio captchas. 
This program will use AI/ML techniques for enhancing and classifying distored audio extracted from Audio Captchas in the wild. 
The whole program will integrate the novel ML audio solutions to extract and respond to audio captchas in the wild.

As at 13/09/2022 the project is still in R&D phase. The summary of items complete is outlined below:

### Captchas
- recaptchv2 is the most progressed with a command line program which can provide transcriptions of input audio data. this performs well in the wilde [INSERT STATISTICS HERE]
- BotDetect (letters and numbers) is still in research stage due to the difficulty of the problem

### Tools
- solve_recaptcha.py: command line program which solves recaptchav2 audio captchas
- spectral_subtraction algorithm: implemented in `r-and-d`

# Audio Captcha Program Overview

The below chart shows a high level overview of the breaking captcha project specifically outlining the goals for Audio Captcha team (seen in green).

![Project Overview](./docs/Breaking%20Captcha.png)

The below chart gives a brief view of the main processes in the proposed solution,

![Program Overview](./docs/Breaking%20Audio%20Captcha.png)

## Data Sources

Key data sources:

- generated data (training and testing purposes): https://demos.telerik.com/aspnet-ajax/captcha/examples/captchaaudiocode/defaultcs.aspx#)
- Extracted data (testing in the wild): see https://github.com/LCroote/breaking-captcha/blob/Audio_CAPTCHA/Teams_T2_2022/Audio_CAPTCHA/data/generated/generate_captcha.py
- recaptcha v2 data (spoken speech): see https://recaptcha-demo.appspot.com/recaptcha-v2-checkbox-explicit.php
- BotDetect data (letters and numbers): see https://captcha.com/audio-captcha-examples.html

## Types of Audio Captcha

- numbers (recaptcha)
- letters and numbers (BotDetect)
- spoken (recaptchav2)

## AI/ML Approach
The approach taken so far in this project is to use large fine-tuned and pretrained models to enhance the audio (such as speechbrain audio enhancement) and then transcribe using large fine-tuned and pretrained models. This has lead to some promising results for speech focused audio captchas but poor results for captchas that are dominated by letteers and numbers. 

## Resources
The resources we currently use are tensorflow, pytorch. The HugginFace (https://huggingface.co/) being the main source of pretrained models. ssrc-1.33 is used for upsampling audio files to 16KHz efficiently for use with models mainly trained in this range. Currently in the works is using Google Cloud platform (GCP) to act as a transcriber.

### Data
Due to the size of the data, this could not be uploaded. Email the Audio breaking captcha team (details provided above) for data.

## References

Ravanelli, M., Parcollet, T., Plantinga, P., Rouhe, A., Cornell, S., Lugosch, L., … Bengio, Y. (2021). SpeechBrain: A General-Purpose Speech Toolkit. arXiv [eess.AS]. Ανακτήθηκε από http://arxiv.org/abs/2106.04624

Fu, S.-W., Yu, C., Hsieh, T.-A., Plantinga, P., Ravanelli, M., Lu, X., & Tsao, Y. (2021). MetricGAN+: An Improved Version of MetricGAN for Speech Enhancement. arXiv preprint arXiv:2104. 03538.

Bagchi, D., Plantinga, P., Stiff, A., & Fosler-Lussier, E. (2018). Spectral Feature Mapping with Mimic Loss for Robust Speech Recognition. IEEE Conference on Audio, Speech, and Signal Processing (ICASSP).

Wang, C., Tang, Y., Ma, X., Wu, A., Okhonko, D., & Pino, J. (2020). fairseq S2T: Fast Speech-to-Text Modeling with fairseq. Proceedings of the 2020 Conference of the Asian Chapter of the Association for Computational Linguistics (AACL): System Demonstrations.


## Usage and Guides

### File structure
#### data
Project specific data and scripts used to generate data.
#### docs
Project specific documentation.
#### r-and-d
Folder containing all R&D for audio processing and classification. Models are trained and tested here before deploying in program.

- recaptcha.ipynb: solving recaptchav2
- transformers.ipynb: investigating transformer based solutions for solving audio captchas. 
    - This has a focus on BotDetect data
    - Implementation of workable spectral subtraction algorithm

#### captcha-solver
Main product directory see captcha_solver/README.md.

#### ssrc-1.33
C++ based cmd line program for smoothly upsampling audio files with limited artifacts.




