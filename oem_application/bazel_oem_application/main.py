# First, parse the configuration file
from config_parser import parse_config_file
print("[PRE] Parsing Configuration file")
parse_config_file()
print("[PRE] Configuration file is parsed successfully")

####################################################

# Install all needed Python requirements 
import subprocess
from config_parser import SRC_DIRECTORY_PATH
print("[PRE] Installing needed Python libraries")
subprocess.run(f"pip install -r {SRC_DIRECTORY_PATH}\\requirements.in", stdout=subprocess.DEVNULL)
print("[PRE] Python libraries are installed successfully")

####################################################

from function_implementation import sign_process, upload_process

print("Siging process started!")
sign_success = sign_process()
print(f"Siging process finished successfully!")

if sign_success:
    print("Uploading process started!")
    upload_process()
    print("Uploading process finished successfully!")
