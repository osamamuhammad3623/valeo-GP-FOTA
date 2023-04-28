#include "Config.h"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool taskCompleted = false;

file_paths master_paths {"OEM/Master ECU/Binary/app.txt", "OEM/Master ECU/Security/combined.txt"};
file_paths target1_paths {"OEM/Target 1/Binary/app.txt", "OEM/Target 1/Security/combined.txt"};
file_paths target2_paths {"OEM/Target 2/Binary/app.txt", "OEM/Target 2/Security/combined.txt"};

file_paths ECUs_paths[3] = {master_paths, target1_paths, target2_paths};

String file_to_be_send;

uint8_t major;
uint8_t minor;
uint8_t patch;

uint8_t urgency;
uint8_t targeted_ecus_21m;


void init_all();
void WIFI_Connect();
void FIREBASE_init();
boolean Download_Files();
void Send_update(String path);
size_t get_file_size(target_id id, file_type type);
void FILESYSTEM_init();
void fcsDownloadCallback(FCS_DownloadStatusInfo info);
void listAllFilesInDir(String dir_path);

void setup() 
{
    init_all();
    yield();
}


void loop() 
{
     if (Serial.available() == 4) 
     {
        uint8_t Frame_id = Serial.read();
        Serial.print(Frame_id);
        Serial.print(Serial.available());
        char version[6] = {0x02, major, minor, patch, urgency, targeted_ecus_21m};
        int current_major;
        int current_minor;
        int current_patch;
        int targetID;
        int Filetype;
        int fileSize;

        switch(Frame_id)
        {
          case 0x01: 
            Serial.println("getting version...");
            current_major = Serial.read();
            current_minor = Serial.read();
            current_patch = Serial.read();
            if(current_major > major)
            {
              break;  
            }
            else if(current_major < major)
            {
              Serial.print("update");
              Serial.write(version, 6);
              break;  
            }
            
            if(current_minor > minor)
            {
              break;  
            }
            else if(current_minor < minor)
            {
              Serial.write(version, 6);
              break;  
            }

            if(current_patch > patch)
            {
              break;  
            }
            else if(current_patch < patch)
            {
              Serial.write(version, 6);
              break;  
            }
          
          case 0x03: 
             if(!Download_Files())
             {
               Serial.write(DOWNLOADING_ERROR);
               break;
             }
             Serial.write(PACKAGE_DOWNLOADED);
             break;
      
          case 0x05: 
             targetID = Serial.read();
             Filetype = Serial.read();
             fileSize = (int)get_file_size((target_id)targetID, (file_type)Filetype);
             Serial.write(0x06);
             Serial.write(fileSize);
             break;
          
          case 0x07: 
             Send_update(file_to_be_send);  
             break;       
        }

     }
     Serial.end();
     Serial.begin(UART_BAUDRATE);
     yield();
     delay(1000);
}

//whole system initialization
void init_all(){
  Serial.begin(UART_BAUDRATE);
  //#if _DEBUG_
  //  Serial.begin(UART_BAUDRATE);
  //#endif
  WIFI_Connect();
  FIREBASE_init();
  FILESYSTEM_init();
}

size_t get_file_size(target_id id, file_type type)
{
  Serial.println("calculating file size...");

  size_t filesize;
  File file;

  LittleFS.begin();

  if(type == bin)
  {
    file = LittleFS.open( "/"+ECUs_paths[id].bin, "r");
    file_to_be_send = ECUs_paths[id].bin;
  }
  else if(type == data)
  {
    file = LittleFS.open( "/"+ECUs_paths[id].data, "r");
    file_to_be_send = ECUs_paths[id].data;
  } 
  
  if(file.available()) 
  {
    filesize = file.size();
  }

  return filesize;
}

boolean Download_Files() {
  while(1)
  {
    if (Firebase.ready() && !taskCompleted) 
    {
      taskCompleted = true;

      Serial.println("Download file...");
      
      if(targeted_ecus_21m & 1)
      {
        if (!((Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,master_paths.bin  /* path of remote file stored in the bucket */,master_paths.bin /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))
          || (Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,master_paths.bin  /* path of remote file stored in the bucket */,master_paths.bin /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))))
        {
          Serial.println(fbdo.errorReason());
          return false;
        }
      }

      if(((targeted_ecus_21m >> 1) & 1))
      {
        if (!((Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,target1_paths.bin  /* path of remote file stored in the bucket */,target1_paths.bin /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))
          || (Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,target1_paths.bin  /* path of remote file stored in the bucket */,target1_paths.bin /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))))
        {
          Serial.println(fbdo.errorReason());
          return false;
        }
      }

      if(((targeted_ecus_21m >> 2) & 1))
      {
        if (!((Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,target2_paths.bin  /* path of remote file stored in the bucket */,target2_paths.bin /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))
          || (Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,target2_paths.bin  /* path of remote file stored in the bucket */,target2_paths.bin /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))))
        {
          Serial.println(fbdo.errorReason());
          return false;
        }
      }
      break;
    }
    yield();
  }
  return true;
}

void Send_update(String path) 
{
    Serial.println("sending file...");

    LittleFS.begin();

    File file = LittleFS.open( "/"+path , "r");
  
    while (file.available()) 
    {
      char buf;
    
      size_t bytesRead = file.readBytes(&buf, sizeof(buf));
      Serial.print(buf);
      yield();
    }
    
    file.close();
    LittleFS.end();
}

void FILESYSTEM_init()
{
    LittleFS.begin();

    LittleFS.mkdir("/OEM");

    LittleFS.mkdir("/OEM/Master ECU");
    LittleFS.mkdir("/OEM/Master ECU/Security");
    LittleFS.mkdir("/OEM/Master ECU/Binary");

    LittleFS.mkdir("/OEM/Target 1");
    LittleFS.mkdir("/OEM/Target 1/Security");
    LittleFS.mkdir("/OEM/Target 1/Binary");

    LittleFS.mkdir("/OEM/Target 2");
    LittleFS.mkdir("/OEM/Target 2/Security");
    LittleFS.mkdir("/OEM/Target 2/Binary");

    LittleFS.end();
}

//Wifi initialization Function
void WIFI_Connect() 
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  #if _DEBUG_
    Serial.print("\nConnecting to Wi-Fi");
  #endif
  char counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #if _DEBUG_
      Serial.print(".");
    #endif
    counter++;
    if (counter == 15) {
      break;
    }
  }
  switch (WiFi.status()) {
  case WL_CONNECTED:
    #if _DEBUG_    
      Serial.println("WiFi connected");
      Serial.println(WiFi.SSID());
      Serial.println(WiFi.localIP());
    #endif
    break;
  case WL_NO_SSID_AVAIL:
    #if _DEBUG_
      Serial.println("SSID cannot be reached !");
    #endif
    ESP.restart();
    break;
  case WL_CONNECT_FAILED:
    #if _DEBUG_
      Serial.println("Wrong Password !");
    #endif
    ESP.restart();
    break;
  default:
    #if _DEBUG_      
      Serial.println("Unknown error !");
    #endif
    ESP.restart();
  }
}

//firebase initialization function
void FIREBASE_init() {
  #if _DEBUG_
    Serial.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);
  #endif
  
  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h

  #if defined(ESP8266)
    // required for large file data, increase Rx size as needed.
    fbdo.setBSSLBufferSize(1024 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
  #endif

  /* Assign download buffer size in byte */
  // Data to be downloaded will read as multiple chunks with this size, to compromise between speed and memory used for buffering.
  // The memory from external SRAM/PSRAM will not use in the TCP client internal rx buffer.
  config.fcs.download_buffer_size = 2048;


  
  /* In case the certificate data was used  */
  // config.cert.data = rootCACert;
  // //set the root certificate for each FirebaseData object
  // fbdo.setCert(rootCACert);

  //config.cert.file = "/gtsr1.pem";
  //config.cert.file_storage = mem_storage_type_flash;

  delay(500);
  Firebase.begin(&config, &auth);
  delay(500);
  Firebase.reconnectWiFi(true);

}

// The Firebase Storage download callback function
void fcsDownloadCallback(FCS_DownloadStatusInfo info) {
  #if _DEBUG_
    if (info.status == fb_esp_fcs_download_status_init) {
      Serial.printf("Downloading file %s (%d) to %s\n", info.remoteFileName.c_str(), info.fileSize, info.localFileName.c_str());
    } else if (info.status == fb_esp_fcs_download_status_download) {
      Serial.printf("Downloaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
    } else if (info.status == fb_esp_fcs_download_status_complete) {
      Serial.println("Download completed\n");
    } else if (info.status == fb_esp_fcs_download_status_error) {
      Serial.printf("Download failed, %s\n", info.errorMsg.c_str());
    }
  #endif  
}


//get all files in FileSystem
void listAllFilesInDir(String dir_path) {
  Dir dir = SPIFFS.openDir(dir_path);
  while (dir.next()) {
    if (dir.isFile()) {
    // print file names
      Serial.print("File: ");
      Serial.println(dir_path + dir.fileName());
    }
    if (dir.isDirectory()) {
      // print directory names
      Serial.print("Dir: ");
      Serial.println(dir_path + dir.fileName() + "/");
      // recursive file listing inside new directory
      listAllFilesInDir(dir_path + dir.fileName() + "/");
    }
  }
}
