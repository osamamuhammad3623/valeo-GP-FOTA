SIGNING_SERVER_PASSWORD = "987654321"
SIGNING_SERVER_USERNAME = "Alaa"

image1_info = {
    "img" : 1,          # READ-ONLY
    "size": 0,          # READ-ONLY
    "digest" : "",      # READ-ONLY
    "crc": 0,           # READ-ONLY
    "target" : "Target 2",
    "path" : "D:\\CSE\\GraduationProject\\valeo-GP-FOTA\\oem_application\\bazel_oem_application\\LED2.bin",
    "root_index" : 2
}

image2_info = {
    "img" : 2,          # READ-ONLY
    "size": 0,          # READ-ONLY
    "digest" : "",      # READ-ONLY
    "crc": 0,           # READ-ONLY
    "target" : "",
    "path" : "",
    "root_index" : 0
}

image3_info = {
    "img" : 3,          # READ-ONLY
    "size": 0,          # READ-ONLY
    "digest" : "",      # READ-ONLY
    "crc": 0,           # READ-ONLY
    "target" : "",
    "path" : "",
    "root_index" : 0
}

package_info = {
    "package_version": "5.5.5",
    "urgency" : 0,
    "n_images" : 1,
    "image1_info": image1_info,     # READ-ONLY
    "image2_info": image2_info,     # READ-ONLY
    "image3_info": image3_info      # READ-ONLY
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