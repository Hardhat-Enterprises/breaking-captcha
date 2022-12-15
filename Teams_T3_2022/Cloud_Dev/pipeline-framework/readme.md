# MLFlow Pipeline Framework

# Summary

The deliverable of this working group is a Machine Learning pipeline which;

- Allows ML experimentation testing and tracking
- Centralised collaboration
- storing models in model registry
- Continuous delivery using model registry and deployed models

This pipeline exists to improve the collaboration between R&D developers as well as provide a mechanism for the demo site to be
better maintained and improved. This will allow future cohorts to continue experimenting but be able to update the models serving the demo
site as they are developed. Furthermore, this work can be continued to include more sophisticated staging and production environments without
significantly increasing the amount of technical dept.

# Technical Overview

The pipeline is built largely around an MLflow experiment tracking and model store framework using 

`mlflow` ML experiment tracking <br>
`minio` Large object S3 storage  <br>
`sqlite` metadata storage <br>

The infrastructure is designed to be run on any machine, locally or remotely. This does not rely on dedicated databasing or S3 services, each of these is created in the root directory on the machine. For Example, if run on `GCP` virtual machine in the project directory there will be the local storage for `s3 buckets` as well as `sqlite database` items.   

## Resources

In this project there are 3 main pieces of work

- Machine learning pipeline
- Testing and logging program for mlflow `/testing`
- flask app which demonstrates an improved `text-captcha` model `/flask-app`


# System Requirements

```
unix
python
conda
brew
```

## MlFlow (minion, sqlite)

```
# unix/linux
# 1. Setup
conda env create -f conda_env.yaml # provided in git

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
# 0. Create environment variables
export MINIO_ROOT_USER=breaking_captcha_admin
export MINIO_ROOT_PASSWORD=breaking_captcha
export MLFLOW_S3_ENDPOINT_URL=http://127.0.0.1:9000
export AWS_ACCESS_KEY_ID=breaking_captcha_admin
export AWS_SECRET_ACCESS_KEY=breaking_captcha
export MLFLOW_TRACKING_URI=http://localhost:8000

# 1. Start minio server (see above)
minio server minio_data --console-address ":9001"

# 2. Start Mlflow server (see above)
mlflow server  --port 8000 \
    --backend-store-uri sqlite:///./database/mlflow.db \
    --default-artifact-root s3://breaking-captcha \
    --host 0.0.0.0

# 3. Deploy Registered Model (see above)
mlflow models serve -m "models:/text-captcha/Production" --env-manager "local" -p 8888

# 4. Run Flask app
cd flask-app
pip install -r requirements.txt # This may not work the same on every system
python app.py

# 5. Go to port for localhost
```

**NOTE** For each of the 4 services (minio, mlflow, deployed model, flask app) you need a new terminal and the environment variables existing in that environment.

## Testing Continuous delivery

Update a new model version to Production and see this reflected in demo site. This is continuous delivery rather than continuous deployment since overview and interaction is required rather than automatic updates based on code passing tests.


1. Update new version to production
2. go to localhost:8000
3. Navigate Models > Version 2 (eg. http://0.0.0.0:8000/#/models/text-captcha/versions/2)
4. Update Stage = Production
5. re-run demo site and see new model in use

## Running on Remote VM (linux implementation of Mlflow (minio + sqlite + mlflow))

The below instructions are as done in a Azure VM. Ensure the correct ports allow traffic to make the mlflow and minio user interfaces accessible.

```
# ssh into VM
ssh -i breaking-captcha_key.pem luka@4.197.13.248

# configure network to listen on ports 9000, 9001, 8000
# This can be done in cloud provider [see here](https://learn.microsoft.com/en-us/azure/application-gateway/quick-create-portal)

# download and intall miniconda
wget https://repo.anaconda.com/miniconda/Miniconda3-py39_4.12.0-Linux-x86_64.sh
bash Miniconda3-latest-Linux-x86_64.sh
conda init

# close and reopen VM

# Download git
sudo-apt get git
# clone repo
git clone https://github.com/Hardhat-Enterprises/breaking-captcha.git

# cd into correct directory
cd breaking-captcha/Teams_T3_2022/Cloud_Dev/pipeline-framework/

# remove mac os specific details from conda_env.yaml (change to just installing tensorflow)
vim conda_env.yaml # make change

# create environment
conda env create -f conda_env.yaml
conda activate mlflow-env3

# Download minio
wget https://dl.minio.io/server/minio/release/linux-amd64/minio
chmod +x minio

# Setup environment variables
export MINIO_ROOT_USER=breaking_captcha_admin
export MINIO_ROOT_PASSWORD=breaking_captcha
export MLFLOW_S3_ENDPOINT_URL=http://127.0.0.1:9000
export AWS_ACCESS_KEY_ID=breaking_captcha_admin
export AWS_SECRET_ACCESS_KEY=breaking_captcha

# run minio server
mkdir minio_data
./minio server minio_data --console-address ":9001"

# make sqlite db
mkdir database

# run mlflow server
mlflow server  --port 8000 \
    --backend-store-uri sqlite:///./database/mlflow.db \
    --default-artifact-root s3://breaking-captcha \
    --host 0.0.0.0

# View UI at http://YOUR-VM-IP-ADDRESS:8000
```

## Running on Remote VM (linux implementation of Mlflow (minio + sqlite + mlflow))

The below instructions are as done in a Azure VM.


```
# ssh into VM
ssh -i breaking-captcha_key.pem luka@4.197.13.248

# configure network to listen on ports 9000, 9001, 8000
# This can be done in cloud provider [see here](https://learn.microsoft.com/en-us/azure/application-gateway/quick-create-portal)

# download and intall miniconda
wget https://repo.anaconda.com/miniconda/Miniconda3-py39_4.12.0-Linux-x86_64.sh
bash Miniconda3-latest-Linux-x86_64.sh
conda init

# close and reopen VM

# Download git
sudo-apt get git
# clone repo
git clone https://github.com/Hardhat-Enterprises/breaking-captcha.git

# cd into correct directory
cd breaking-captcha/Teams_T3_2022/Cloud_Dev/pipeline-framework/

# remove mac os specific details from conda_env.yaml (change to just installing tensorflow)
vim conda_env.yaml # make change

# create environment
conda env create -f conda_env.yaml
conda activate mlflow-env3

# Download minio
wget https://dl.minio.io/server/minio/release/linux-amd64/minio
chmod +x minio

# Setup environment variables
export MINIO_ROOT_USER=breaking_captcha_admin
export MINIO_ROOT_PASSWORD=breaking_captcha
export MLFLOW_S3_ENDPOINT_URL=http://127.0.0.1:9000
export AWS_ACCESS_KEY_ID=breaking_captcha_admin
export AWS_SECRET_ACCESS_KEY=breaking_captcha

# run minio server
mkdir minio_data
./minio server minio_data --console-address ":9001"

# make sqlite db
mkdir database

# run mlflow server
mlflow server  --port 8000 \
    --backend-store-uri sqlite:///./database/mlflow.db \
    --default-artifact-root s3://breaking-captcha \
    --host 0.0.0.0

# View UI at http://YOUR-VM-IP-ADDRESS:8000
```


## Debugging Notes

```
# When running serving mlflow model
# error: [WARNING] Worker with pid 71 was terminated due to signal 9
# Restart all servers and boot up flask app second, this coulod be some kind of memory issue
# TODO limit memory allocation for different servers (?)

# Flask app or trainer.py not working
# this may be due to missing pip libraries, this is not solved in depth in this readme because it varies from os to os. 
# This was developed on a MAC with M1 Chip which is why Conda is being used
# Linux systems maybe simpler

# If the program isnt working for unexplained reasons it may be due to environment variables not existing in the shell or terminal you run your programs from

# Sometimes the server reports no boto3 package. Just rerun pip install boto3

```

# Future trimester work

### **TODO** Create Staging and Production environments for proper CI/CD

Current Production Models are deployed and used the demo site. This should in theory be once again split into staging and production which is all supported by mlflow.

### **TODO** Continous Deployment based on newer model versions improving on metrics

This would require using Git or Jenkins to automate the updating of a new Model to `Stage=Production` based on the correct metrics being used. Current CD is continuous delivery due to changes flowing through when user accepts a new version to Production.

### **TODO** Deploy pipeline to production environment (GCP)

The current proof of concept is based locally but built such that it can be deployed remotely. This would be necessary if the Team is to use this solution.

### **TODO** Uptake of mlflow described for R&D Teams for remote and local use

Encourage uptake so that teams working on models can collaborate correctly.

### **TODO** Deploy new flask app using prediction endpoint

The existing demo site is not fit for purpose of using the pipeline. The current demo site needs to be replaced with an upgrade version similar to that provide in this directory in `flask-app`.

### **TODO** Containerise solution 

Current solution has some operational risk depending on system and user. A containerised solution should be able to pboot up with 1 command.
