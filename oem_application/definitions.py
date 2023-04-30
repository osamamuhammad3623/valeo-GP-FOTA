'''
NOTES:
target has the following values:
    Master ECU
    Target 1
    Target 2

imageN_info["path"] is captured in load_image_bin
'''

image1_info = {
    "img" : 1,
    "path" : "",
    # metadata info
    "root_index" : 0,
    "size": 0,
    "digest" : "",
    # app info
    "crc": 0,
    "target" : ""
}

image2_info = {
    "img" : 2,
    "path" : "",
    # metadata info
    "root_index" : 0,
    "size": 0,
    "digest" : "",
    # app info
    "crc": 0,
    "target" :0
}

image3_info = {
    "img" : 3,
    "path" : "",
    # metadata info
    "root_index" : 0,
    "size": 0,
    "digest" : "",
    # app info
    "crc": 0,
    "target" :0
}

package_info = {
    "package_version": "",
    "urgency" : 0,
    "n_images" : 0,
    "image1_info": image1_info,
    "image2_info": image2_info,
    "image3_info": image3_info
}

realtime_attributes = {
    "is_urgent" : 0,
    "pckg_version" : "",
    "targeted_ecus_21m" : 0,
    "crc" : {
        "master" : {
            "image" : 0,
            "update_data" : 0
        },
        "target_1" : {
            "image" : 0,
            "update_data" : 0
        },
        "target_2" : {
            "image" : 0,
            "update_data" : 0
        }
    }
}