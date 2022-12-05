import pyrebase

# Firebase configs
configurations = {

}

# global variables
firebase = pyrebase.initialize_app(configurations)
authen = firebase.auth()
database_handle = firebase.database()
storage_handle = firebase.storage()

