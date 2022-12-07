# MLFlow Pipeline Framework

# Overview

The pipeline is built largely around an MLflow experiment tracking and model store framework using 

`mlflow` ML experiment tracking <br>
`minio` Large object S3 storage  <br>
`sqlite` metadata storage <br>

The infrastructure is designed to be run on any machine, locally or remotely. This does not rely on dedicated databasing or S3 services, each of these is created in the root directory on the machine. For Example, if run on `GCP` virtual machine in the project directory there will be the local storage for `s3 buckets` as well as `sqlite database` items.   

# System Requirements

```
unix
python
conda
```

## MlFlow (minion, sqlite)

```
# unix/linux
# 1. Setup
conda env create -f conda_env.yaml

# 2. Activate
conda activate mlflow-env3

# 3. minio (for s3 buckets)
brew install minio/stable/minio

# 4. Set environment variables
export MINIO_ROOT_USER=breaking_captcha_admin
export MINIO_ROOT_PASSWORD=breaking_captcha
export MLFLOW_S3_ENDPOINT_URL=http://127.0.0.1:9000
export AWS_ACCESS_KEY_ID=breaking_captcha_admin
export AWS_SECRET_ACCESS_KEY=breaking_captcha

# 5. create local storage for minio data
mkdir minio_data

# 6. start minio server
minio server minio_data --console-address ":9001"
# 7. Open provided link and create bucket called breaking-captcha

# 8. Run mlflow with local backend (sqlite) and local minio buckets (minio)
mlflow server  --port 8000 \
    --backend-store-uri sqlite:///./database/mlflow.db \
    --default-artifact-root s3://breaking-captcha \
    --host 0.0.0.0
```

## Training and experimentation

See `testing/trainer.py` for format on how to interact with mlflow. For this we use [tensorflow logging](https://www.mlflow.org/docs/latest/python_api/mlflow.tensorflow.html) to log the right things as well as models.

```
# 1. get sample data for training place it into testing/samples
# 2. run
python trainer.py

```

## Serving Models

Using Mlflow we can serve [registered models](https://www.mlflow.org/docs/latest/models.html#built-in-deployment-tools)

1. Using mlflow ui register the desired model and set the desired production version to `stage = Production`
2. run the below code (this will deploy the desired model which is Status = Production)

```
#!/usr/bin/env sh

# Set environment variable for the tracking URL where the Model Registry resides
export MLFLOW_TRACKING_URI=http://localhost:8000

# Serve the production model from the model registry
mlflow models serve -m "models:/text-captcha/Production" --env-manager "local" -p 8888
```

## Running App Locally

To run the flask demo app locally use the below steps. In summary this launches `mlflow` server which uses `sqlite` for metadata and `minio` for artifact storage. The App is a flask app that queries the deployed model endpoint. 

```
# 1. Start minio server (see above)
# 2. Start Mlflow server (see above)
# 3. Deploy Registered Model (see above)
# 4. Run Flask app
cd flask-app
python app.py
# 5. Go to port for localhost
```

## Testing Continuous delivery

Update a new model version to Production and see this reflected in demo site. This is continuous delivery rather than continuous deployment since overview and interaction is required rather than automatic updates based on code passing tests.


1. Update new version to production
2. go to localhost:8000
3. Navigate Models > Version 2 (eg. http://0.0.0.0:8000/#/models/text-captcha/versions/2)
4. Update Stage = Production
5. re-run demo site and see new model in use


## Debugging Notes

```
# When running serving mlflow model
# error: [WARNING] Worker with pid 71 was terminated due to signal 9
# Restart all servers and boot up flask app second, this coulod be some kind of memory issue
# TODO limit memory allocation for different servers (?)
```

# Future trimester work

### **TODO** Create Staging and Production environments for proper CI/CD

Current Production Models are deployed and used the demo site. This should in theory be once again split into staging and production which is all supported by mlflow.

### **TODO** Continous Deployment based on newer model versions improving on metrics

This would require using Git or Jenkins to automate the updating of a new Model to `Stage=Production` based on the correct metrics being used.

### **TODO** Deploy pipeline to production environment (GCP)

The current proof of concept is based locally but built such that it can be deployed remotely. This would be necessary if the Team is to use this solution.

### **TODO** Uptake of mlflow described for R&D Teams for remote and local use

Encourage uptake so that teams working on models can collaborate correctly.

### **TODO** Deploy new flask app using prediction endpoint

The existing demo site is not fit for purpose of using the pipeline. The current demo site needs to be replaced with an upgrade version similar to that provide in this directory in `flask-app`.
