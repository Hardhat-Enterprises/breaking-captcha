import os
os.environ ['GOOGLE_APPLICATION_CREDENTIALS'] = 'key.json'

# Imports the Google Cloud client library
from google.cloud import speech

# Instantiates a client
client = speech.SpeechClient()

# The name of the audio file to transcribe
gcs_uri = "gs://sit-22t2-breaking-capt-8718903_cloudbuild/3rpx.wav"

audio = speech.RecognitionAudio(uri=gcs_uri)

config = speech.RecognitionConfig(
    sample_rate_hertz=8000,
    language_code="en-US",
    model="latest_short",
    speech_contexts= [
        {
            "phrases":["3 b "],
            "boost": 20
        },{
            "phrases":["3 r"],
            "boost": 20
        },{
            "phrases":["3"],
            "boost": 20
        },{
            "phrases":["b"],
            "boost": 20
        },{
            "phrases":[" "],
            "boost": 20
        }
    ]
)

# Detects speech in the audio file
response = client.recognize(config=config, audio=audio)

for result in response.results:
    print(result)
    # print("Transcript: {}".format(result.alternatives[0]))