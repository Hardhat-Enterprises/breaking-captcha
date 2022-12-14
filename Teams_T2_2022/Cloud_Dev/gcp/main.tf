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

resource "google_storage_bucket" "cf-source-bucket" {
  name     = "cf-source-python-files"
  location = "US"
}

data "archive_file" "captcha_function_files" {
  type = "zip"

  output_path = "${path.cwd}/functions_code/source_files.zip"
  source_dir = "${path.cwd}/functions_code/source_files"
}

resource "google_storage_bucket_object" "archive" {
  name   = "index.zip"
  bucket = google_storage_bucket.cf-source-bucket.name
  source = data.archive_file.captcha_function_files.output_path
}

resource "google_cloudfunctions_function" "function" {
  name        = "function-test"
  description = "My function"
  runtime     = "python38"

  available_memory_mb   = 128
  source_archive_bucket = google_storage_bucket.cf-source-bucket.name
  source_archive_object = google_storage_bucket_object.archive.name
  trigger_http          = true
  entry_point           = "main"
}