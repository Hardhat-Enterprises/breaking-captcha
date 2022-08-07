# Enable Compute Engine
resource "google_project_service" "project_compute" {
  project                    = var.project_id
  service                    = "compute.googleapis.com"
  disable_dependent_services = true
  disable_on_destroy         = true
}

# Enable VPC Access
resource "google_project_service" "project_vpcaccess" {
  project                    = var.project_id
  service                    = "vpcaccess.googleapis.com"
  disable_dependent_services = true
  disable_on_destroy         = true
}