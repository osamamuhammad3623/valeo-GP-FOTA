import pyrebase

firebaseConfig = {
  "apiKey": "AIzaSyBQ1Q2OPmZXkDQgMNkyeaV-FwdAR-Kphqc",
  "authDomain": "oem-application.firebaseapp.com",
  "databaseURL": "https://oem-application-default-rtdb.firebaseio.com",
  "projectId": "oem-application",
  "storageBucket": "oem-application.appspot.com",
  "messagingSenderId": "526497884209",
  "appId": "1:526497884209:web:3c98198729aac226638d10",
  "measurementId": "G-LFP70JP2X8",

  "serviceAccount" : "serviceAccountKey.json"
};

firebase = pyrebase.initialize_app(firebaseConfig)
storage = firebase.storage()
rlt_db = firebase.database()


def firebase_upload_file(firebase_path, file_path):
  storage.child(firebase_path).put(file_path)


def firebase_update_attributes(attributes_dict):
  rlt_db.child("is_urgent").set(attributes_dict["is_urgent"])
  rlt_db.child("pckg_version").set(attributes_dict["pckg_version"])
  rlt_db.child("targeted_ecus_21m").set(attributes_dict["targeted_ecus_21m"])

  # update CRC calculations for all targets
  rlt_db.child("CRC/master/image").set(attributes_dict["crc"]["master"]["image"])
  rlt_db.child("CRC/master/update_data").set(attributes_dict["crc"]["master"]["update_data"])

  rlt_db.child("CRC/target_1/image").set(attributes_dict["crc"]["target_1"]["image"])
  rlt_db.child("CRC/target_1/update_data").set(attributes_dict["crc"]["target_1"]["update_data"])

  rlt_db.child("CRC/target_2/image").set(attributes_dict["crc"]["target_2"]["image"])
  rlt_db.child("CRC/target_2/update_data").set(attributes_dict["crc"]["target_2"]["update_data"])
