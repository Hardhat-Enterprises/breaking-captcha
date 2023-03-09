# Audio analysis & ML
from asyncio import subprocess
import torch
import torchaudio
from speechbrain.pretrained import SpectralMaskEnhancement
from transformers import Speech2TextProcessor, Speech2TextForConditionalGeneration
from transformers import pipeline
from transformers import Wav2Vec2Processor, Wav2Vec2ForCTC
from speechbrain.pretrained import WaveformEnhancement
import numpy as np

#################
# Enhancement
#################
# Speech brain metricgan-plus-voicebank
def enhance_audio_metrican(path_to_file):
    # setup spectral enhancement from speechbrain
    enhance_model = SpectralMaskEnhancement.from_hparams("models/pretrained_models/metricgan-plus-voicebank")

    # Load and add fake batch dimension
    original_ = enhance_model.load_audio(path_to_file).unsqueeze(0)

    # Enhance
    enhanced_ = enhance_model.enhance_batch(original_, lengths=torch.tensor([1.]))

    return enhanced_ # This is 2 levels deep tensor [[s1, s2, ..., sn]]


# Speech brain metricgan-plus-voicebank
def enhance_audio_mimic(path_to_file):
    # setup spectral enhancement from speechbrain
    enhance_model = WaveformEnhancement.from_hparams("models/pretrained_models/mtl-mimic-voicebank")
    # enhanced = enhance_model.enhance_file(path_to_file)
    # Load and add fake batch dimension
    original_ = enhance_model.load_audio(path_to_file).unsqueeze(0)

    # Enhance
    enhanced_ = enhance_model.enhance_batch(original_, lengths=torch.tensor([1.]))

    return enhanced_ # This is 2 levels deep tensor [[s1, s2, ..., sn]]


#################
# ASR
#################
# s2t-small-librispeech-asr
def transcribe_s2t(audio_sample, sample_rate=16000):
    # Instantiate model
    model = Speech2TextForConditionalGeneration.from_pretrained("models/pretrained_models/facebook/model/s2t-small-librispeech-asr")
    # Processor = Featurise + tokenise
    processor = Speech2TextProcessor.from_pretrained("models/pretrained_models/facebook/processor/s2t-small-librispeech-asr")
    inputs = processor(audio_sample, sampling_rate=sample_rate, return_tensors="pt")
    generated_ids = model.generate(inputs["input_features"], attention_mask=inputs["attention_mask"])
    # Transcribe
    transcription = processor.batch_decode(generated_ids)
    
    return [s.lower() for s in transcription]

# s2t-small-librispeech-asr
def transcribe_wave2vec(audio_sample, sample_rate=16000):
    # load model and tokenizer
    processor = Wav2Vec2Processor.from_pretrained("models/pretrained_models/facebook/processor/wav2vec2-base-960h")
    model = Wav2Vec2ForCTC.from_pretrained("models/pretrained_models/facebook/model/wav2vec2-base-960h")

    # tokenize
    input_values = processor(audio_sample, sampling_rate=sample_rate, return_tensors="pt", padding="longest").input_values  # Batch size 1

    # retrieve logits
    logits = model(input_values).logits

    # take argmax and decode
    predicted_ids = torch.argmax(logits, dim=-1)
    transcription = processor.batch_decode(predicted_ids)
    return [s.lower() for s in transcription]

#################
# Solver
#################


def solve_captcha(path_to_file, enhancement_functions, asr_functions):
    """ 
    Description:
    This function takes an input audio file path, a list of enhancement functions and a list of asr functions and returns a string of most common 
    that corresponds to the most common solution of testing each enhancement and asr combination
    """

    transcriptions = []
    for enhance in enhancement_functions:
        enhanced_audio = enhance(path_to_file)
        for asr in asr_functions:
            transcription = asr(enhanced_audio[0])
            transcriptions.append(transcription)
    
    # Extract most common if competing this will take the first
    values, counts = np.unique(transcriptions, return_counts=True)
    ind = np.argmax(counts)
    most_common = values[ind]

    return most_common#, transcriptions


# solutions = solve_captcha('../data/recaptcha/adifferentpassionsfrom.wav', [enhance_audio_metrican, enhance_audio_mimic], [transcribe_s2t, transcribe_wave2vec])
# print(solutions)

def main():
    # Audio analysis & ML
    import torch
    import torchaudio
    from speechbrain.pretrained import SpectralMaskEnhancement
    from transformers import Speech2TextProcessor, Speech2TextForConditionalGeneration
    from transformers import pipeline
    from transformers import Wav2Vec2Processor, Wav2Vec2ForCTC
    from speechbrain.pretrained import WaveformEnhancement
    import numpy as np
    import subprocess
    import sys
    import os

    user_input = input('Input path to data: ')
    path = str(user_input)
    
    # Check for non wav format
    if user_input.find('.wav') < 0:
        subprocess.call(['ffmpeg', '-i', path, './captcha_audio.wav'])
        path = 'captcha_audio.wav'

    result = solve_captcha(path, [enhance_audio_metrican, enhance_audio_mimic], [transcribe_s2t, transcribe_wave2vec])

    print(result)

if __name__ == "__main__":
    main()
