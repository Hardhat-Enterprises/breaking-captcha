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

resource "google_pubsub_topic" "ps1" {
  name = "bc_ps1"

  labels = {
    foo = "bar"
  }

  message_retention_duration = "86600s"
}
