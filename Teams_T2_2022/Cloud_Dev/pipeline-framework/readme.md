# MLFlow Pipeline Framework

# Technologies

# System Requirements

# Setup

# On MAC
```
# Conda environment
# Setup
conda env create -f conda_env.yaml
# Activate
conda activate mlflow-env

# Testing
# mlfow server

# minio (for s3 buckets)
brew install minio/stable/minio

# Set environment variables
export MINIO_ROOT_USER=breaking_captcha_admin
export MINIO_ROOT_PASSWORD=breaking_captcha
export MLFLOW_S3_ENDPOINT_URL=http://192.168.1.108:9000
export AWS_ACCESS_KEY_ID=breaking_captcha_admin
export AWS_SECRET_ACCESS_KEY=breaking_captcha

# start server
minio server ./minio-server

# backend store (database)
# sqllite

# Run mlflow with local backend and local minio buckets
# mlflow server --port 8000 --backend-store-uri sqlite:///./database/mlflow.db --default-artifact-root s3://minio-server/ 

```

## Locally

## Remote (VM)