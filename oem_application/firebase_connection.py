import pyrebase

firebaseConfig = {
  "apiKey": "AIzaSyBJ6TnE49i7H7Ei7vFIlJFTeWdWCnXRwMg",
  "authDomain": "gradproject-45283.firebaseapp.com",
  "databaseURL": "https://gradproject-45283-default-rtdb.firebaseio.com",
  "projectId": "gradproject-45283",
  "storageBucket": "gradproject-45283.appspot.com",
  "messagingSenderId": "368683276168",
  "appId": "1:368683276168:web:65a720c48560af20fa4c3d",
  "measurementId": "G-QHK7HB7SLE",

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
  rlt_db.child("CRC/Master ECU/image").set(attributes_dict["crc"]["Master ECU"]["image"])
  rlt_db.child("CRC/Master ECU/update_data").set(attributes_dict["crc"]["Master ECU"]["update_data"])

  rlt_db.child("CRC/Target 1/image").set(attributes_dict["crc"]["Target 1"]["image"])
  rlt_db.child("CRC/Target 1/update_data").set(attributes_dict["crc"]["Target 1"]["update_data"])

  rlt_db.child("CRC/Target 2/image").set(attributes_dict["crc"]["Target 2"]["image"])
  rlt_db.child("CRC/Target 2/update_data").set(attributes_dict["crc"]["Target 2"]["update_data"])