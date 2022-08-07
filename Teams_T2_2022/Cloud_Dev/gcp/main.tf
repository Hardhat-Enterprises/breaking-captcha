# Bucket, function, pub/sub and vision resource creation goes here

resource "google_storage_bucket" "state_bucket" {
  name          = "${var.project_id}-bucket-tfstate"
  force_destroy = false
  location      = "US"
  storage_class = "STANDARD"
  versioning {
    enabled = true
  }
}