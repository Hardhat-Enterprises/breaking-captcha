# Access Connector
resource "google_vpc_access_connector" "vpc_connector" {
  name          = var.vpc_access_connector_name
  ip_cidr_range = var.vpcac_ip_cidr_range
  network       = var.vpc_network_name
}