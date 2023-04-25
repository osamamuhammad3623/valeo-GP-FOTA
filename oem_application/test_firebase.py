import pyrebase

firebaseConfig = {
  "apiKey": "AIzaSyBMbSSPfUkFcHH7Jkz0NZWx2v27POuFa00",
  "authDomain": "test-with-qt-6d806.firebaseapp.com",
  "databaseURL": "https://oem-application-default-rtdb.firebaseio.com/",
  "projectId": "test-with-qt-6d806",
  "storageBucket": "test-with-qt-6d806.appspot.com",
  "messagingSenderId": "489250365454",
  "appId": "1:489250365454:web:63f931de12b6fa491fdc5f"
};

firebase = pyrebase.initialize_app(firebaseConfig)
db = firebase.database()

db.child("Version").update({"ABCD":5})