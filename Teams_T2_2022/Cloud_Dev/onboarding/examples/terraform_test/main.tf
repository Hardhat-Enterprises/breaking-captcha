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

resource "google_storage_bucket" "test-bucket" {
<<<<<<< HEAD
<<<<<<< HEAD
  name          = "jets_test_bucket"
=======
  name          = "test_bucket"
>>>>>>> T3handover
=======
  name          = "test_bucket"
>>>>>>> 1c9a8283e759c31d1f198cb364363b54e666252e
  location      = "US"
  force_destroy = true
}
