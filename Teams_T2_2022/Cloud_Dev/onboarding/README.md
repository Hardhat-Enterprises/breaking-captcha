
# Setting up Terraform & Cloud CLI
## Mac

### Terraform
Terraform can be installed via Homebrew for Mac

>brew tap hashicorp/tap
>brew install hashicorp/tap/terraform

Next run this command to verify your installation:
>terraform -version

Next for syntax and error highlighting install the VS Code Extenion Terraform by Hasicorp

### Gcloud CLI

1. Visit [here](https://cloud.google.com/sdk/docs/install) and select the MacOS tab.
2. Download the package for your Mac
3. Unzip the file
4. >cd /google-cloud-sdk
5. >./install.sh

## Windows
### Terraform

Follow the steps [here](https://learn.hashicorp.com/tutorials/terraform/install-cli) to install on windows.

### Gcloud CLI
1. Visit [here](https://cloud.google.com/sdk/docs/install) and select the Windows tab.
2. Copy the PowerShell code
3. > (New-Object  Net.WebClient).DownloadFile("https://dl.google.com/dl/cloudsdk/channels/rapid/GoogleCloudSDKInstaller.exe",  "$env:Temp\GoogleCloudSDKInstaller.exe")  & $env:Temp\GoogleCloudSDKInstaller.exe
4. Should be something like the above.
5. Follow the steps through the installer.


# Deploying Terraform

To test if your Terraform is working, there is a test file included here [main.tf](https://github.com/cyber-panther/breaking-captcha/blob/main/Teams_T2_2022/Cloud_Dev/onboarding/examples/terraform_test/main.tf)

1. Download the main.tf file locally
2. In a Terminal/CMD window in the directory where the main.tf file is stored type the following commands:
3. >terraform init
4. >gcloud auth application-default login
5. >gcloud auth application-default set-quota-project sit-22t2-breaking-capt-8718903
6. >terraform plan
7. The Terraform plan should say 1 resource to add and it should be a google-cloud-storage bucket. If this is correct.
8. >terraform apply
9. When verified that the bucket is created. Run
10. >terraform destroy 

This formatting of main.tf should not be followed, it is horrible practice, the only purpose of the file is to ensure that your Terraform is setup and connected to our project. Read why [here](https://github.com/cyber-panther/breaking-captcha/blob/main/Teams_T2_2022/Cloud_Dev/onboarding/examples/terraform_test/README.md)
