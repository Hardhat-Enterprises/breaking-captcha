# Steps

1. `cd` into directory
2. Create `Dockerfile`
3. Create `cloudbuild.yaml`
4. run `glcoud builds submit --config cloudbuild.yaml` .
5. Go to your google cloud console and enable Kubernetes cluster
    1. Choose `Ubuntu with Docker` image type where machine type has at least 4.5 GB memory
    2. In Security `Allow full access to all Cloud APIs`
6. **alternatively** run 

```
gcloud beta container --project "sit-22t2-breaking-capt-8718903" clusters create "cluster-1" --zone "us-central1-c" --no-enable-basic-auth --cluster-version "1.23.12-gke.100" --release-channel "regular" --machine-type "e2-standard-2" --image-type "COS_CONTAINERD" --disk-type "pd-standard" --disk-size "100" --metadata disable-legacy-endpoints=true --scopes "https://www.googleapis.com/auth/cloud-platform" --max-pods-per-node "110" --num-nodes "2" --logging=SYSTEM,WORKLOAD --monitoring=SYSTEM --enable-ip-alias --network "projects/sit-22t2-breaking-capt-8718903/global/networks/default" --subnetwork "projects/sit-22t2-breaking-capt-8718903/regions/us-central1/subnetworks/default" --no-enable-intra-node-visibility --default-max-pods-per-node "110" --no-enable-master-authorized-networks --addons HorizontalPodAutoscaling,HttpLoadBalancing,GcePersistentDiskCsiDriver --enable-autoupgrade --enable-autorepair --max-surge-upgrade 1 --max-unavailable-upgrade 0 --enable-shielded-nodes --node-locations "us-central1-c"
```

7. Go to AI Platform
8. Select Pipelines and create a new instance
9. configure Kubeflow
10. Click Accept and Deploy