from PyQt5.QtWidgets import QFileDialog
import requests
import base64
from support_functions import *
import firebase_connection
from combine_bins import combine_security_files
from definitions import *


'''
A function to create the metadata of an image
'''
def write_metadata_file(image_info):
    img_num = image_info["img"]
    pckg_version = package_info["package_version"]
    digest = image_info["digest"]
    root_index = image_info["root_index"]
    size = str(image_info["size"])
    # for metadata parsing [abdo's 7antafa]:
    if len(size) == 5:
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


'''
A function to get the package info [Package & application Versions, Urgency & images] from GUI
'''
def read_package_info(window):
    global package_info # to reference the global variable, instead of creating a local one
    package_info["package_version"] = window.version.text()
    package_info["urgency"] = window.urgency.isChecked()

    n_img = window.n_images.value()
    package_info["n_images"] = n_img

    # read every image's info
    if n_img >= 1:
        image1_info["root_index"] = window.image_root_index_1.value()
        image1_info["size"] = get_file_size(window.image_path_1.text())
        image1_info["digest"] = get_file_digest(window.image_path_1.text())
        image1_info["crc"] = get_file_crc(window.image_path_1.text())
        image1_info["target"] = window.image_target_1.currentText()

    if n_img >= 2:
        image2_info["root_index"] = window.image_root_index_2.value()
        image2_info["size"] = get_file_size(window.image_path_2.text())
        image2_info["digest"] = get_file_digest(window.image_path_2.text())
        image2_info["crc"] = get_file_crc(window.image_path_2.text())
        image2_info["target"] = window.image_target_2.currentText()

    if n_img == 3:
        image3_info["root_index"] = window.image_root_index_3.value()
        image3_info["size"] = get_file_size(window.image_path_3.text())
        image3_info["digest"] = get_file_digest(window.image_path_3.text())
        image3_info["crc"] = get_file_crc(window.image_path_3.text())
        image3_info["target"] = window.image_target_3.currentText()


'''
A function that connects to the signing server to sign the metadata,
    then it writes the cert. & signatures

    return: True for success, False for failure
'''
def sign_metadata(img_num, server_password):
    url = "https://signing-server.onrender.com"
    
    #post request
    binary_data = open(f"metadata_{img_num}.txt","rb").read()
    base64_data = base64.b64encode(binary_data).decode('utf-8')
    url2 = url+"/sign"
    post_data ={"name": "metadata.txt", "content":base64_data, "password": server_password}
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
def sign_process(window):
    images_signed=0
    # read inputs from GUI
    read_package_info(window)
    server_pass = window.server_password.text()
    if len(server_pass) < 5:
        server_pass = "123456789"

    # create metadata file for each image, then sign it,
    #       then combine the security bins in one binary
    n_img = window.n_images.value()

    for i in range(1, n_img+1):
        image= image1_info
        if i == 2:
            image = image2_info
        elif i == 3:
            image = image3_info
        
        write_metadata_file(image)
        if sign_metadata(i, server_pass):

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
            images_signed += 1
        else:
            window.error_msg.setText(f"Application {i} is NOT signed. Signing process stopped")
            return

    # check if all package images are processed successfully
    if images_signed == package_info["n_images"]:
        window.error_msg.setText("All package images are signed successfully")


'''
A function to:
- upload package binaries
- upload applications security bins
- update the package attributes

Inputs: number of images
'''
def upload_process(window):
    current_pckg = package_info["package_version"]
    targeted_ecus = 0

    for i in range (1,window.n_images.value()+1):
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

        # upload image files
        firebase_connection.firebase_upload_file(f"{security_dir}combined.bin", f"combined_{i}.bin")
        firebase_connection.firebase_upload_file(f"{binary_dir}app.bin", image["path"])

    # read package attributes
    realtime_attributes["is_urgent"] = package_info["urgency"]
    realtime_attributes["pckg_version"] = current_pckg
    realtime_attributes["targeted_ecus_21m"] = targeted_ecus
    # update package attributes
    firebase_connection.firebase_update_attributes(realtime_attributes)

    window.error_msg.setText("Uploading process & updating Realtime attributes is done!")



'''
A function to load the binary image from a local device.
'''
def load_image_bin(window, image_number):
    # open a file dialog & set it to select only .bin files
    file_dialog = QFileDialog()
    file_dialog.setFileMode(QFileDialog.AnyFile)
    file_dialog.setNameFilter("Binaries (*.bin)")
    file_dialog.exec_()
    # get the selected file path
    if (len(file_dialog.selectedFiles()) >= 1):
        file_path = file_dialog.selectedFiles()[0]

        #set the file path of the corresponding image number
        if image_number == 1:
            window.image_path_1.setText(file_path)
            image1_info["path"] = file_path
        elif image_number == 2:
            window.image_path_2.setText(file_path)
            image2_info["path"] = file_path
        elif image_number == 3:
            window.image_path_3.setText(file_path)
            image3_info["path"] = file_path


def show_image_info(window, image_count):
    window.image_file_2.setEnabled(False)
    window.image_path_2.setEnabled(False)
    window.image_root_index_2.setEnabled(False)
    window.image_target_2.setEnabled(False)

    window.image_file_3.setEnabled(False)
    window.image_path_3.setEnabled(False)
    window.image_root_index_3.setEnabled(False)
    window.image_target_3.setEnabled(False)

    if image_count >= 2:
        window.image_file_2.setEnabled(True)
        window.image_path_2.setEnabled(True)
        window.image_root_index_2.setEnabled(True)
        window.image_target_2.setEnabled(True)

        if image_count == 3:
            window.image_file_3.setEnabled(True)
            window.image_path_3.setEnabled(True)
            window.image_root_index_3.setEnabled(True)
            window.image_target_3.setEnabled(True)