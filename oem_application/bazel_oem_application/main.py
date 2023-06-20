from function_implementation import *


print("Siging process started!")
sign_success = sign_process()
print(f"Siging process finished successfully!")

if sign_success:
    print("Uploading process started!")
    upload_process()
    print("Uploading process finished successfully!")
