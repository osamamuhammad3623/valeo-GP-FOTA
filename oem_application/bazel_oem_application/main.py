# Install all Python requirements first 
import os
from package_config import SRC_DIRECTORY_PATH
os.system(f"pip install -r {SRC_DIRECTORY_PATH}\\requirements.in")

####################################################
from function_implementation import sign_process, upload_process

print("Siging process started!")
sign_success = sign_process()
print(f"Siging process finished successfully!")

if sign_success:
    print("Uploading process started!")
    upload_process()
    print("Uploading process finished successfully!")
