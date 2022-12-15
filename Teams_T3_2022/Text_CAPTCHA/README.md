# Breakign Text CAPTCHA

Working Github: https://github.com/nbonair/Text-Captcha 

## Progress Summary 

In trimester 3 2022, Text-CAPTCHA team was mainly focus on developing a non-segmentation model to transcript the text CAPTCHA image into text format. Our initial plan was delivering a CNN model to extract the text from image, with the target for model accuracy is above 80%. Nevertheless, Text-CAPTCHA team has ended up beyond this goal, by implement a neuron network model using CNN and RNN for character recognition, and CTC techniques to transcript the feature vector into text format while dealing with complex background of the CAPTCHA. This model succeeded in solving the former dataset with an 99% accuracy on different testing data, without any advance image process on the original dataset. However, due to lack of resources, we could only complete the architecture for the model, and have done with training phase and evaluation on single grayscale dataset. In the near future, our goal is completing the comprehensive training for text recognition model, develop a fully functional module for image processing to generate compatible input data and deliver a program for end-user, to break any text-captcha from their raw image. Finally, by having a great accuracy with this network architecture, the currently used technology could open-up a huge room for project development, such as hand-writing recognition approach, or object detection to break complicated image CAPTCHA. 

 

## Future plan

From the above progress, our team proposes development plan as below: 

**Application development**

- Implement comprehensive training phases with complex dataset (complex background/text font, color image) 

- Complete image processing module for all raw image input 

- Develop an application to break input image CAPTCHA from end-user 

- Integrate to company pipeline 

**R&D tasks:**

- Transfer learning research 

- Develop model to extract hand-writing data from different languages (requires more dataset) 

- R&D phase for object detection for image CAPTCHA 

- Further research and development for new approach in computer vision field 
