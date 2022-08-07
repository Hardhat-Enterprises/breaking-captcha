terraform {
  required_providers {
    google = {
      source = "hashicorp/google"
      version = "4.30.0"
    }
  }
  backend "local" {}
}

provider "google" {
  project = var.project_id
  region = var.region
}