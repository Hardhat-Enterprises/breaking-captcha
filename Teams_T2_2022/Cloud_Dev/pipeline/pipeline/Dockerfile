# start by pulling the python image
FROM tensorflow/tensorflow

# copy the requirements file into the image
COPY ./requirements.txt .

RUN pip install -r requirements.txt

# # Allow statements and log messages to immediately appear in the Knative logs
# ENV PYTHONUNBUFFERED True

# # Copy local code to the container image.
# ENV APP_HOME /app
# WORKDIR $APP_HOME
# COPY . ./

# # install the dependencies and packages in the requirements file
# ADD requirements.txt .

# RUN apt-get update
# RUN apt-get install ffmpeg libsm6 libxext6 libgl1 -y

# RUN python -m pip install --upgrade pip && \
#     pip install -r requirements.txt

# # configure the container to run in an executed manner
# ENTRYPOINT [ "python", "app.py" ]

# EXPOSE $PORT

# CMD exec gunicorn --bind :$PORT --workers 1 --threads 8 --timeout 0 main:app