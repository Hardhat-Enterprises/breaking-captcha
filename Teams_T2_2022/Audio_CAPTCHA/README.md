# Breaking Audio Captcha
This is the Project directory for Breaking Audio Captcha project for Deakin Capstone.

Contributers:

- Luka Croote (lcroote@deakin.edu.au)
- Ridwan Ganiyu (rbganiyu@deakin.edu.au)
- Theodore Hrysomallis (thrysomallis@deakin.edu.au)
- Josh Tolding (jtolding@deakin.edu.au)

## Summary
The aim of this stream is to develop a program for breaking audio captchas. 
This program will use AI/ML techniques for enhancing and classifying distored audio extracted from Audio Captchas in the wild. 
The whole program will integrate the novel ML audio solutions to extract and respond to audio captchas in the wild.

## Audio Captcha Program Overview

The below chart shows a high level overview of the breaking captcha project specifically outlining the goals for Audio Captcha team (seen in green).

![Project Overview](./docs/Breaking%20Captcha.png)

The below chart gives a brief view of the main processes in the proposed solution,

![Program Overview](./docs/Breaking%20Audio%20Captcha.png)

## Data Sources

Key data sources:

- generated data (training and testing purposes): https://demos.telerik.com/aspnet-ajax/captcha/examples/captchaaudiocode/defaultcs.aspx#)
- Extracted data (testing in the wild): see https://github.com/LCroote/breaking-captcha/blob/Audio_CAPTCHA/Teams_T2_2022/Audio_CAPTCHA/data/generated/generate_captcha.py

## AI/ML Approach

## Resources

## References

## Usage and Guides

### File structure
#### data
Project specific data and scripts used to generate data.
#### docs
Project specific documentation.
#### research-and-development
Folder containing all R&D for audio processing and classification. Models are trained and tested here before deploying in program.
#### captcha-solver
Python package for solving Audio Captchas.

- Models for processing are versioned and stored in `captcha-solver/models`. 
- Entry point to the program: `captcha-solver/captcha-solver.py`.
- testing: `captcha-solver/test`

