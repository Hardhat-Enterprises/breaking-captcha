terraform {
  required_providers {
    google = {
      source = "hashicorp/google"
      version = "4.30.0"
    }
  }
}

provider "google" {
  project = "sit-22t2-breaking-capt-8718903"
  region = "australia-southeast2"
}

resource "google_storage_bucket" "test_bucket" {
  name          = "test_bucket"
  location      = "US"
  force_destroy = true
}
