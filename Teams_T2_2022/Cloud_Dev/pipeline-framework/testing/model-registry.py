import mlflow.pyfunc
from mlflow import MlflowClient

# client = MlflowClient()
# client.transition_model_version_stage(
#     name="text-captcha",
#     version=1,
#     stage="Staging"
# )

from pprint import pprint

client = MlflowClient()
for rm in client.search_registered_models():
    pprint(dict(rm), indent=4)