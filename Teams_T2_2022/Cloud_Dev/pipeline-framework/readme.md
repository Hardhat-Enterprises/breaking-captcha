# MLFlow Pipeline Framework

# Overview

The pipeline is built largely around an MLflow experiment tracking and model store framework using 

`mlflow` ML experiment tracking <br>
`minio` Large object S3 storage  <br>
`sqlite` metadata storage <br>

The infrastructure is designed to be run on any machine, locally or remotely. This does not rely on dedicated databasing or S3 services, each of these is created in the root directory on the machine. For Example, if run on `GCP` virtual machine in the project directory there will be the local storage for `s3 buckets` as well as `sqlite database` items.   

# Technologies

# System Requirements

# Setup


# On MAC
```
# Setup
conda env create -f conda_env.yaml

# Activate
conda activate mlflow-env3

# minio (for s3 buckets)
brew install minio/stable/minio

# Set environment variables
export MINIO_ROOT_USER=breaking_captcha_admin
export MINIO_ROOT_PASSWORD=breaking_captcha
export MLFLOW_S3_ENDPOINT_URL=http://127.0.0.1:9000
export AWS_ACCESS_KEY_ID=breaking_captcha_admin
export AWS_SECRET_ACCESS_KEY=breaking_captcha

# create local storage for minio data
mkdir minio_data

# start minio server
minio server minio_data --console-address ":9001"

# Run mlflow with local backend (sqlite) and local minio buckets (minio)
mlflow server  --port 8000 \
    --backend-store-uri sqlite:///./database/mlflow.db \
    --default-artifact-root s3://breaking-captcha \
    --host 0.0.0.0
```


## Locally

## Remote (VM)