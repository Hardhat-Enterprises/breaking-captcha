# Handover T3 2022

This document outlines necessary handover information for the Cloud engineering team.

# Key Locations

- Work folder [pipeline-framework](https://github.com/Hardhat-Enterprises/breaking-captcha/tree/Cloud_Dev/Teams_T2_2022/Cloud_Dev/pipeline-framework)

## Trimester Focus

The focus in trimester 3 was implementing a Machine learning pipeline using the best technology available. This work was begun to

1. Improve collaboration and R&D practices within the team
2. Improve build and deployment for the demo website

## Deliverables

The team delivered a program which could be run locally or on a remote vm which setup a Machine learning pipeline using `mlflow`. The program uses `mlflow` for model and experiment tracking server, `minio` for artifact storage and `sqlite` for metadata storage. The Demo runs on flask and uses a deployed endpoint from the `mlflow` model registry. [See here](https://github.com/Hardhat-Enterprises/breaking-captcha/tree/Cloud_Dev/Teams_T2_2022/Cloud_Dev/pipeline-framework) for a guide on how to deploy the pipeline.

## Outstanding Work

    - Acceptance testing for use with wider team
    - Permanent deployment to GCP for real collaboration
    - Improve Docker deployment using docker compose
    - Bring Audio Breaking into new app structure
    - Better mode robust CI/CD worflow

Contributers:

    - Luka Croote lcroote@deakin.edu.au
