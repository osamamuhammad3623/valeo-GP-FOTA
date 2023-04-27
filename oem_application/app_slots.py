from PyQt5.QtWidgets import QFileDialog, QMessageBox
import hashlib
import os
from definitions import *
import requests
import base64
import firebase_connection


def get_file_size(file_path):
    return os.path.getsize(file_path)


def get_file_digest(file_path):
    with open(file_path, 'rb', buffering=0) as f:
        return hashlib.file_digest(f, 'sha256').digest()


def write_metadata_file(image_info):
    pckg_version = package_info["package_version"]
    digest = image_info["digest"]
    root_index = image_info["root_index"]
    version = image_info["version"]
    size = str(image_info["size"])
    # for metadata parsing [abdo's 7antafa]
    if len(size) == 5:
        size += "  "
    elif len(size) == 6:
        size += " "
    
    data = f"version:{pckg_version}.{version}.1 ,size:{size},rootIndex:{root_index} ,App_Hash:"
    metadata_file = open(f'metadata.txt', 'w') # write data in normal mode
    metadata_file.write(data)
    metadata_file.close()
    metadata_file_byte = open(f'metadata.txt', 'ab') # write digest in bytes mode
    metadata_file_byte.write(digest)
    metadata_file_byte.close()


def read_package_info(window):
    # read package info from UI (version, urgency & #images)

    global package_info # to reference the global variable, instead of creating a local one
    package_info["package_version"] = window.version.value()
    package_info["urgency"] = window.urgency.isChecked()

    n_img = window.n_images.value()
    package_info["n_images"] = n_img

    # read every image's info
    if n_img >= 1:
        image1_info["img"] = 1
        image1_info["version"] = window.image_version_1.value()
        image1_info["root_index"] = window.image_root_index_1.value()
        image1_info["size"] = get_file_size(window.image_path_1.text())
        image1_info["digest"] = get_file_digest(window.image_path_1.text())
        image1_info["crc"] = int(window.image_crc_1.text())
        image1_info["target"] = window.image_target_1.currentText()

    if n_img >= 2:
        image2_info["img"] = 2
        image2_info["version"] = window.image_version_2.value()
        image2_info["root_index"] = window.image_root_index_2.value()
        image2_info["size"] = get_file_size(window.image_path_2.text())
        image2_info["digest"] = get_file_digest(window.image_path_2.text())
        image2_info["crc"] = int(window.image_crc_2.text())
        image2_info["target"] = window.image_target_2.currentText()

    if n_img == 3:
        image3_info["img"] = 3
        image3_info["version"] = window.image_version_3.value()
        image3_info["root_index"] = window.image_root_index_3.value()
        image3_info["size"] = get_file_size(window.image_path_3.text())
        image3_info["digest"] = get_file_digest(window.image_path_3.text())
        image3_info["crc"] = int(window.image_crc_3.text())
        image3_info["target"] = window.image_target_3.currentText()


def sign_metadata():
    url = "https://signing-server.onrender.com"
    
    #post request
    binary_data = open("metadata.txt","rb").read()
    base64_data = base64.b64encode(binary_data).decode('utf-8')
    url2 = url+"/sign"
    post_data ={"name": "metadata.txt", "content":base64_data, "password": "123456789"}
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


def process_input(window):
    # read inputs from GUI
    read_package_info(window)

    # create metadata file for each image & sign it
    n_img = window.n_images.value()
    if n_img >= 1:
        write_metadata_file(image1_info)
        if sign_metadata():
            # Upload files
            firebase_connection.firebase_upload_file("ROOT__cert.bin")
            firebase_connection.firebase_upload_file("SB_cert.bin")
            firebase_connection.firebase_upload_file("signature.bin")
            window.error_msg.setText("Application 1 signed & uploaded successfully.")
        else:
            window.error_msg.setText("Application 1 is NOT signed.")

    if n_img >= 2:
        write_metadata_file(image2_info)
        sign_metadata()
    if n_img == 3:
        write_metadata_file(image3_info)
        sign_metadata()


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
        elif image_number == 2:
            window.image_path_2.setText(file_path)
        elif image_number == 3:
            window.image_path_3.setText(file_path)


def show_image_info(window, image_count):

    window.image_file_2.setEnabled(False)
    window.image_path_2.setEnabled(False)
    window.image_crc_2.setEnabled(False)
    window.image_root_index_2.setEnabled(False)
    window.image_version_2.setEnabled(False)
    window.image_target_2.setEnabled(False)

    window.image_file_3.setEnabled(False)
    window.image_path_3.setEnabled(False)
    window.image_crc_3.setEnabled(False)
    window.image_root_index_3.setEnabled(False)
    window.image_version_3.setEnabled(False)
    window.image_target_3.setEnabled(False)

    if image_count == 2 or image_count == 3:
        window.image_file_2.setEnabled(True)
        window.image_path_2.setEnabled(True)
        window.image_crc_2.setEnabled(True)
        window.image_root_index_2.setEnabled(True)
        window.image_version_2.setEnabled(True)
        window.image_target_2.setEnabled(True)

        if image_count == 3:
            window.image_file_3.setEnabled(True)
            window.image_path_3.setEnabled(True)
            window.image_crc_3.setEnabled(True)
            window.image_root_index_3.setEnabled(True)
            window.image_version_3.setEnabled(True)
            window.image_target_3.setEnabled(True)
