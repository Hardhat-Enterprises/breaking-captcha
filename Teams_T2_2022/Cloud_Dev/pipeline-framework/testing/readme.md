# Testing ML pipeline

The testing folder contains tests for trainer (running experiments on mlflow server with sqlite and minio backend) and predictions using deployed models from model registry. See [pipeline-framework](https://github.com/LCroote/breaking-captcha/tree/pipeline/Teams_T2_2022/Cloud_Dev/pipeline-framework).

# Training

```
python trainer.py [path_to_training_data (file containing .png images)]
# See http://localhost:8000
```

# Predictions

Note: the output of test predictor is index location of predictor letter or number.

```
python test_predictor.py
```

