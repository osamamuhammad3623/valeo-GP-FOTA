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


def firebase_upload_file(file_path):
  storage.child(file_path).put(file_path)