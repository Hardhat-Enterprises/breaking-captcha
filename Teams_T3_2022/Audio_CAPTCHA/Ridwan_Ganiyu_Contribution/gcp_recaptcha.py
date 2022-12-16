import os
os.environ ['GOOGLE_APPLICATION_CREDENTIALS'] = 'key.json'

# Imports the Google Cloud client library
from google.cloud import speech

# Instantiates a client variable
client = speech.SpeechClient()

# The name of the audio file to transcribe
gcs_uri = "gs://sit-22t2-breaking-capt-8718903_cloudbuild/adifferentpassionsfrom.mp3"

# Use GCP RecognitionAudio() function on audio file, and set audio file in variable
audio = speech.RecognitionAudio(uri=gcs_uri)

# Set configuration to use on audio file
config = speech.RecognitionConfig(
    sample_rate_hertz=16000,
    language_code="en-US",
)

# Detects speech in the audio file
response = client.recognize(config=config, audio=audio)

for result in response.results:
    print("{}".format(result.alternatives[0]))