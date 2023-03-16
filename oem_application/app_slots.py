from PyQt5.QtWidgets import QFileDialog
import hashlib
import os
from definitions import *


def get_file_size(file_path):
    return os.path.getsize(file_path)

def get_file_digest(file_path):
    with open(file_path, 'rb', buffering=0) as f:
        return hashlib.file_digest(f, 'sha256').hexdigest()

def write_metadata_file(image_info):
    digest = image_info["digest"]
    root_index = image_info["root_index"]
    version = image_info["version"]
    size = str(image_info["size"])
    if len(size) == 5:
        size += "  "
    elif len(size) == 6:
        size += " "
    
    data = f"version:{package_version}.{version}.1 ,size:{size},rootIndex:{root_index} ,App_Hash:{digest}"
    image_number = image_info["img"]
    metadata_file = open(f'app{image_number}_md.bin', 'w')
    metadata_file.write(data)


def read_package_info(window):
    global package_version # to reference the global variable, instead of creating a local one
    package_version = window.version.value()

    global urgency
    urgency = window.urgency.isChecked()

    n_img = window.n_images.value()
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


def process_input(window):
    # read inputs
    read_package_info(window)

    # create metadata file for each image
    n_img = window.n_images.value()
    if n_img >= 1:
        write_metadata_file(image1_info)
    if n_img >= 2:
        write_metadata_file(image2_info)
    if n_img == 3:
        write_metadata_file(image3_info)


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