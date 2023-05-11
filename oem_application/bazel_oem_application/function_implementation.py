import requests
import base64
import os, hashlib, binascii
import firebase_connection
from combine_bins import combine_security_files
from package_config import *


'''
A function to create the metadata of an image
'''
def write_metadata_file(image_info):
    img_num = image_info["img"]
    print(f"[IMAGE {img_num}] Creating metadata file.")
    pckg_version = package_info["package_version"]
    digest = image_info["digest"]
    root_index = image_info["root_index"]
    size = str(image_info["size"])
    # for metadata parsing [abdo's 7antafa]:
    if len(size) == 4:
        size += "   "
    elif len(size) == 5:
        size += "  "
    elif len(size) == 6:
        size += " "
    
    data = f"version:{pckg_version} ,size:{size},rootIndex:{root_index} ,App_Hash:"
    metadata_file = open(f'metadata_{img_num}.txt', 'w') # write data in normal mode
    metadata_file.write(data)
    metadata_file.close()
    metadata_file_byte = open(f'metadata_{img_num}.txt', 'ab') # write digest in bytes mode
    metadata_file_byte.write(digest)
    metadata_file_byte.close()
    print(f"[IMAGE {img_num}] Metadata file created successfully.")


def get_package_data():
    global package_info # to reference the global variable, instead of creating a local one
    print("[PACKAGE] Calculating images CRCs & digests")
    # read every image's info
    if package_info["n_images"] >= 1:
        path = image1_info["path"]
        image1_info["size"] = get_file_size(path)
        image1_info["digest"] = get_file_digest(path)
        image1_info["crc"] = get_file_crc(path)

    if package_info["n_images"] >= 2:
        path = image2_info["path"]
        image1_info["size"] = get_file_size(path)
        image1_info["digest"] = get_file_digest(path)
        image1_info["crc"] = get_file_crc(path)

    if package_info["n_images"] == 3:
        path = image3_info["path"]
        image1_info["size"] = get_file_size(path)
        image1_info["digest"] = get_file_digest(path)
        image1_info["crc"] = get_file_crc(path)


'''
A function that connects to the signing server to sign the metadata,
    then it writes the cert. & signatures

    return: True for success, False for failure
'''
def sign_metadata(img_num):
    url = "https://signing-server.onrender.com"
    
    #post request
    binary_data = open(f"metadata_{img_num}.txt","rb").read()
    base64_data = base64.b64encode(binary_data).decode('utf-8')
    url2 = url+"/sign"
    post_data ={"name": "metadata.txt", "content":base64_data,"username":SIGNING_SERVER_USERNAME,"password": SIGNING_SERVER_PASSWORD}
    response = requests.post(url2, json=post_data)

    data =response.json()
    if(response.status_code ==200):
        Root_der_cert_base64 = data["ROOT_DER_CERT"]
        SB_der_cert_base64 = data["SB_DER_CERT"]
        signature_base64 = data["SIGNATURE"]
        Root_der_cert_bin = base64.b64decode(Root_der_cert_base64)
        SB_der_cert_bin = base64.b64decode(SB_der_cert_base64)
        signature_bin = base64.b64decode(signature_base64)
        with open('ROOT__cert.bin', 'wb') as file:
            file.write(Root_der_cert_bin)
        with open('SB_cert.bin', 'wb') as file:
            file.write(SB_der_cert_bin)
        with open('signature.bin', 'wb') as file:
            file.write(signature_bin)
        
        return True
    else:
        print(data["detail"])
        return False

'''
when the user presses Signing Request button:
1. read user input
2. create metadata file for every image.
3. sign the metadata file of each image.
'''
def sign_process():
    images_signed=0
    # read inputs from configurations
    get_package_data()

    # create metadata file for each image, then sign it,
    #       then combine the security bins in one binary
    n_img = package_info["n_images"]

    for i in range(1, n_img+1):
        image= image1_info
        if i == 2:
            image = image2_info
        elif i == 3:
            image = image3_info
        
        write_metadata_file(image)
        if sign_metadata(i):
            print(f"[IMAGE {i}] Image is signed successfully")
            # create certificates_metadata.bin for the image
            root_cert_size_with_spaces = str(get_file_size("ROOT__cert.bin"))
            # [abdo's 7antafa]:
            if len(root_cert_size_with_spaces) == 3:
                root_cert_size_with_spaces += "  "
            elif len(root_cert_size_with_spaces) == 4:
                root_cert_size_with_spaces += " "

            sb_size_with_spaces = str(get_file_size("SB_cert.bin"))
            # [abdo's 7antafa]:
            if len(sb_size_with_spaces) == 3:
                sb_size_with_spaces += " "

            data = f"Root_Cert_Size:{root_cert_size_with_spaces},SB_Cert_Size:{sb_size_with_spaces}"
            metadata_file = open("certificates_metadata.bin", 'w') # write data in normal mode
            metadata_file.write(data)
            metadata_file.close()

            # combine security files
            combine_security_files(i)
            print(f"[IMAGE {i}] Security certificates are combined into a single binary")
            images_signed += 1
        else:
            print(f"[IMAGE {i}] Application is NOT signed. Signing process stopped")
            return False

    # check if all package images are processed successfully
    if images_signed == package_info["n_images"]:
        print("[PACKAGE] All package images are signed successfully")
        return True


'''
A function to:
- upload package binaries
- upload applications security bins
- update the package attributes

Inputs: number of images
'''
def upload_process():
    current_pckg = package_info["package_version"]
    targeted_ecus = 0

    for i in range (1,package_info["n_images"]+1):
        image = image1_info
        if i == 2:
            image = image2_info
        elif i == 3:
            image = image3_info
        
        # get the targeted ECU
        target = image["target"]

        if target == "Master ECU":
            targeted_ecus = targeted_ecus | (1<<0)
        elif target == "Target 1":
            targeted_ecus = targeted_ecus | (1<<1)
        elif target == "Target 2":
            targeted_ecus = targeted_ecus | (1<<2)

        # determine directories
        security_dir = f"OEM/{current_pckg}/{target}/Secure/"
        binary_dir = f"OEM/{current_pckg}/{target}/Binary/"

        # set the CRC of the image & combined file in realtime attributes
        realtime_attributes["crc"][target]["image"] = image["crc"]
        realtime_attributes["crc"][target]["update_data"] = get_file_crc(f"combined_{i}.bin")

        print(f"[IMAGE {i}] Uploading the image files to Firebase")
        # upload image files
        firebase_connection.firebase_upload_file(f"{security_dir}combined.bin", f"combined_{i}.bin")
        firebase_connection.firebase_upload_file(f"{binary_dir}app.bin", image["path"])

    # read package attributes
    realtime_attributes["is_urgent"] = package_info["urgency"]
    realtime_attributes["pckg_version"] = current_pckg
    realtime_attributes["targeted_ecus_21m"] = targeted_ecus
    # update package attributes
    print("[PACKAGE] Updating package attributes in Firebase")
    firebase_connection.firebase_update_attributes(realtime_attributes)

    print("Uploading process & updating Realtime attributes is done!")


'''
A function to get the size of a file
'''
def get_file_size(file_path):
    return os.path.getsize(file_path)


'''
A function to calculate the digest of a file
'''
def get_file_digest(file_path):
    with open(file_path, 'rb', buffering=0) as f:
        return hashlib.file_digest(f, 'sha256').digest()


'''
A function to calculate the CRC of a file
'''
def get_file_crc(file_path):
    buf = open(file_path,'rb').read()
    hash = binascii.crc32(buf) & 0xFFFFFFFF
    return int("%08X" % hash, 16)
