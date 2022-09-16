# Next steps for R & D

Currently we have developed a working solution for speech clip based audio captchas ([captcha solver README](https://github.com/LCroote/breaking-captcha/blob/Audio_CAPTCHA/Teams_T2_2022/Audio_CAPTCHA/captcha-solver/README.md)). The ultimate goal of this stream is to solve the most challenging audio captchas, these are the ones who are actively deployed and used. Other captchas have lower noise and also work well when simply tackled with speecht2text algorithms. Furthermore, letters and digits are a challenge but in some we can implement a simple word mapping heuristic to solve this as outlined [here](https://github.com/ecthros/uncaptcha).

Below I will outline next steps to solve easier number and letter based captchas using a simple heuristic. I will also outline the next steps for solving more challenging letter and number problems like [BotDetect](https://captcha.com/audio-captcha-examples.html).

## Word Mapping Heuristic

Simply put when letter and numbers captcha is put through asr we receive the "word" form of these numbers and letter. For example

`11F340 -> won one eff thre for vero`

as you can see, due to the noise we do not get clear words, so a heuristic must be designed to convert say "vero" to "zero" to "0". 

- The most simple approach is to build a growing dictionary of mappings based on training and observation. 
- Another is to build a similarity calculator between words and there correct spellings. 


## BotDetect

- Segment audio data by taking pairs of samples where peaks in speech are clear
    1. This can be done in a number of ways, using libraries in python that implement "split on silence" algorithms, however this can be challenging with noise
    2. Best approach I believe is to take the derivative of sample amplitudes above zero and below, and extract pairs of points where the derivative is non zero since noise periods between speech will have more consistent results around 0.
    3. Another method I would try is to use an image classificaiton approach to classify areas where speech is apparent. This comes with added challenge of labelling data.
    4. Finally looking at `transformers.ipynb` we can see that using a simple clustering algoirthm on fixed width widows may group together speech samples thus avoiding labelling.

- Train classifier on segments to identify ([0-9A-Z])
- Combine results into final string