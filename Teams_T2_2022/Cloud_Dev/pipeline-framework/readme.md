# MLFlow Pipeline Framework

# Overview

The pipeline is built largely around an MLflow experiment tracking and model store framework using 

`mlflow` ML experiment tracking <br>
`minio` Large object S3 storage  <br>
`sqlite` metadata storage <br>

The infrastructure is designed to be run on any machine, locally or remotely. This does not rely on dedicated databasing or S3 services, each of these is created in the root directory on the machine. For Example, if run on `GCP` virtual machine in the project directory there will be the local storage for `s3 buckets` as well as `sqlite database` items.   

# Technologies

# System Requirements

```
python
conda
pyenv
```

# Setup


# On MAC
```
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


## Locally

**Serving Models**
```
#!/usr/bin/env sh

# Set environment variable for the tracking URL where the Model Registry resides
export MLFLOW_TRACKING_URI=http://localhost:8000

# Serve the production model from the model registry
mlflow models serve -m "models:/text-captcha/latest" --env-manager "local" -p 8888

```

## Remote (VM)

# Running App Locally

To run the flask demo app locally use the below steps. In summary this launches `mlflow` server which uses `sqlite` for metadata and `minio` for artifact storage. The App is a flask app that queries the deployed model endpoint. 

```
1. Start minio server (see above)
2. Start Mlflow server (see above)
3. Deploy Registered Model (see above)
4. Run Flask app
cd flask-app
python app.py
5. Go to port for localhost
```