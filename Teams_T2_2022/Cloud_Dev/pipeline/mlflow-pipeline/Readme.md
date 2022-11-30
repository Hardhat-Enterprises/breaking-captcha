# MLFlow Setup

```
# Create service account
1. gcloud iam service-accounts create mlflow-tracking-sa --description="Service Account to run the MLFLow tracking server" --display-name="MLFlow tracking SA"
# Create Bucket 
2. gsutil mb gs://breaking-captcha-bucket
# Create SQL instance 
3. gcloud sql instances create mlflow-backend --tier=db-f1-micro --region=us-central1 --storage-type=SSD
4. gcloud sql databases create mlflow --instance=mlflow-backend
# Add roles
5. gsutil iam ch 'serviceAccount:mlflow-tracking-sa@sit-22t2-breaking-capt-8718903.iam.gserviceaccount.com:roles/storage.admin' gs://breaking-captcha-bucket
6. gcloud projects add-iam-policy-binding sit-22t2-breaking-capt-8718903 --member='serviceAccount:mlflow-tracking-sa@sit-22t2-breaking-capt-8718903.iam.gserviceaccount.com' --role=roles/cloudsql.editor
``` d