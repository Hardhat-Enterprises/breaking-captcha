<img src="https://github.com/jethall1/breaking-captcha/blob/Cloud_Dev/Teams_T2_2022/Cloud_Dev/cloud_logo.png?raw=true" width="100" height="100">
<h1> <strong>Cloud Team</strong> </h1>
<h2> <strong>INTRODUCTION</strong> </h2>
<p>Welcome to the Cloud Team Repo, this is a readme file thats provides detailed technical insight into each directories contents within the GCS project. 
Below is a table of what's covered in this readme document.  </p>

|Table of contents|
|-----------------|
| 1. GCP folder                 |
| 2. Onboarding Details                |
| 3. Web Application folder                |
| 4. pipeline-framework                |



# T3 Handover    

Due to IT issues, both updated demo site and pipeline-framework need to be offically launched in GCP in future trimesters. See the respective folders and handover documents for more detail.

### updated-design
Contributer: Koshila Akalanka Dodantenna (kdodantenna@deakin.edu.au)

In the `webapp/updated-design` you can find the updated webapp design which significantly improved the UI of the demo site. This has not yet been implemented in GCP.

### Machine Learning Pipeline 

See `pipeline-framework`.



# T2 Handover for continuation

<p>This project was run over 12 weeks and required all members of the group to contribute to their designated role </p> 

<details><summary>Group Details</summary>
    <p>Member details of the group can be seen in the table below</p> 
    
   | Senior Members | Junior Members |
   | -------------- | -------------- |
   | Jet Hall       | Agrim Gautam           |
   | Connor Gent    | Luka Croote         |
   | Cecilia Sammuit      | Terry Skliros         |
    
</details>

## **GCP Details:** ##
<p>Inside of GCP is majority of the terraform files utilized in this project that helped create the GCS storage buckets. <strong>These include:</strong></p>
 
        - function code folder
        - api.tf 
        - gcptf.tfvars
        - main.tf
        - network.tf
        - provider.tf
        - variables.tf
  
**functions folder consists of the following:**
  * main.py ~ Which is the source code of the cloud functions 
  * requirements.txt ~ list of python libraries required by main.py to run. 

**Main.tf**:
  * This file is created to establish the main declarations of the developed environment. 

**Variables.tf**:
  * This files is used to deine and store the variables that can then be declared when constructing the GCS storage buckets. 

**API.tf**:
  * Required to esablish communication/access between terraform and GCS.  

## **Onboarding Details:** ##
<p>Inside of onboarding is the onboarding guides for the this project
</p>


## **The web app folder:** ##
<p>In the web app folder is the container code to deploy the docker container
    Containment <strong>files within the constructed web folder</strong> include:

        - Notebook file
        - Templates file
        - Dockerfile
        - app.py 
        - requirements
        
</p>
<p> The <strong>Docker file:</strong>  
      The project's docker container is crucial to this GCP project as it shortens the process of building, testing and deploying.</p>

Why use a docker container?
  * Utilizing a Docker container has enabled us to store every important facets within the container. 
      * they are essential to the running of the web application. 
  * Examples of files stored within include: templates, standarded images and training images. 


    
