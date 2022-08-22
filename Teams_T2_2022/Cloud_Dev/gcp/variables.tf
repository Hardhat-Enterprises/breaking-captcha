variable "project_id" {
  description = "Project ID"
  type        = string
  default     = "sit-22t2-breaking-capt-8718903"
}

variable "region" {
  description = "Australia Region"
  type        = string
  default     = "us-east1"
}

variable "vpc_access_connector_name" {
  description = "VPC Access Connector Name"
  type        = string
}

variable "vpcac_ip_cidr_range" {
  description = "Subnet Network IP CIDR Name"
  type        = string
}

variable "vpc_network_name" {
  description = "VPC Network Name"
  type        = string
  default     = "default"
}