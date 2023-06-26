import os
'''
Possible Values for image info:
"target"           -> "MasterECU", "Target 1", "Target 2"
"root_index"       -> 1, 2
'''

SIGNING_SERVER_PASSWORD = ""
SIGNING_SERVER_USERNAME = ""
SRC_DIRECTORY_PATH = ""

image1_info = {
    "img" : 1,
    "size": 0,
    "digest" : "",
    "crc": 0,
    "target" : "",
    "path" : "",
    "root_index" : 0
}

image2_info = {
    "img" : 2,
    "size": 0,
    "digest" : "",
    "crc": 0,
    "target" : "",
    "path" : "",
    "root_index" : 0
}

image3_info = {
    "img" : 3,
    "size": 0,
    "digest" : "",
    "crc": 0,
    "target" : "",
    "path" : "",
    "root_index" : 0
}

package_info = {
    "package_version": "",
    "urgency" : 0,
    "n_images" : 0,
    "image1_info": image1_info,
    "image2_info": image2_info,
    "image3_info": image3_info
}


# READ-ONLY DICTIONARY
realtime_attributes = {
    "is_urgent" : 0,
    "pckg_version" : "",
    "targeted_ecus_21m" : 0,
    "crc" : {
        "Master ECU" : {
            "image" : 0,
            "update_data" : 0
        },
        "Target 1" : {
            "image" : 0,
            "update_data" : 0
        },
        "Target 2" : {
            "image" : 0,
            "update_data" : 0
        }
    }
}


def parse_config_file():
    with open(os.path.join(os.path.dirname(__file__), "ota_config"), 'r') as file:
        config = file.readlines()
        for line in config:
            line =line.replace("\n", "").split(" ")
            config_key = line[0]
            config_value = line[-1]

            if config_key == "SRC_DIRECTORY_PATH":
                global SRC_DIRECTORY_PATH
                SRC_DIRECTORY_PATH = config_value

            if config_key == "SIGNING_SERVER_PASSWORD":
                global SIGNING_SERVER_PASSWORD
                SIGNING_SERVER_PASSWORD = config_value

            if config_key == "SIGNING_SERVER_USERNAME":
                global SIGNING_SERVER_USERNAME
                SIGNING_SERVER_USERNAME = config_value

            # image 1 parsing
            if config_key == "IMAGE_1_TARGET":
                image1_info["target"] = line[-2] +" "+ line[-1]

            if config_key == "IMAGE_1_BINARY_PATH":
                image1_info["path"] = config_value

            if config_key == "IMAGE_1_ROOT_INDEX":
                image1_info["root_index"] = int(config_value)

            # image 2 parsing
            if config_key == "IMAGE_2_TARGET":
                image2_info["target"] = line[-2] +" "+ line[-1]

            if config_key == "IMAGE_2_BINARY_PATH":
                image2_info["path"] = config_value

            if config_key == "IMAGE_2_ROOT_INDEX":
                image2_info["root_index"] = int(config_value)

            # image 3 parsing
            if config_key == "IMAGE_3_TARGET":
                image3_info["target"] = line[-2] +" "+ line[-1]

            if config_key == "IMAGE_3_BINARY_PATH":
                image3_info["path"] = config_value

            if config_key == "IMAGE_3_ROOT_INDEX":
                image3_info["root_index"] = int(config_value)

            # package parsing
            if config_key == "PACKAGE_VERSION":
                package_info["package_version"] = config_value

            if config_key == "PACKAGE_URGENCY":
                package_info["urgency"] = int(config_value)

            if config_key == "PACKAGE_IMAGES_NUM":
                package_info["n_images"] = int(config_value)