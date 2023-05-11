#include "Config.h"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool taskCompleted = false;

file_paths master_paths {"OEM/MasterECU/Binary/app.txt", "OEM/MasterECU/Secure/meta.txt"};
file_paths target1_paths {"OEM/Target 1/Binary/app.txt", "OEM/Target 1/Secure/meta.txt"};
file_paths target2_paths {"OEM/Target 2/Binary/app.txt", "OEM/Target 2/Secure/meta.txt"};

file_paths ECUs_paths[3] = {master_paths, target1_paths, target2_paths};

String file_to_be_send;

//version of package currently running on ECUs
uint8_t ECUs_major;
uint8_t ECUs_minor;
uint8_t ECUs_patch;

//pckg_version
uint8_t pckg_major;
uint8_t pckg_minor;
uint8_t pckg_patch;

//urgency and detection
int is_urgent;
int targeted_ecus_21m;

//CRC_Att structs
struct CRC_Att master_crc;
struct CRC_Att target1_crc;
struct CRC_Att target2_crc;

//storage vairables for CRC_Att structs
int master_crc_image;
int master_crc_update_data;
int target1_crc_image;
int target1_crc_update_data;
int target2_crc_image;
int target2_crc_update_data;

uint16_t chunk_size;

uint8_t no_of_bytes;

uint8_t flag = 0;

void init_all();
void WIFI_Connect();
void FIREBASE_init();
boolean Download_Files();
void Send_update(String path);
size_t get_file_size(target_id id, file_type type);
void set_chunk_size(uint16_t first_byte, uint8_t second_byte);
void FILESYSTEM_init();
void fcsDownloadCallback(FCS_DownloadStatusInfo info);
void listAllFilesInDir(String dir_path);
void free_serial_buffer();
void get_attributes();
boolean check_pckg_version();

void setup() 
{
    init_all();
}


void loop() 
{
    if(flag)
    {
      if(check_pckg_version())
      {
        char pckg_version_frame[6] = {0x02, (uint8_t)is_urgent, pckg_major, pckg_minor, pckg_patch, (uint8_t)targeted_ecus_21m};
        Serial.write(pckg_version_frame, 6);
        flag = 0;
      }
    }

     if (Serial.available() > 0) 
     {
        uint8_t Frame_id = Serial.read();
        char size[4];

        int targetID;
        int Filetype;
        int fileSize;

        uint16_t first_chunk_size_byte;
        uint8_t second_chunk_size_byte;

        switch(Frame_id)
        {
          case 0x01: 
            ECUs_major = Serial.read();
            ECUs_minor = Serial.read();
            ECUs_patch = Serial.read();
            flag = 1;
            break;
          
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
             size[0] = 0x06;
             size[1]= ((fileSize >> 16)  & 0xFF);
             size[2]= ((fileSize >> 8) & 0xFF);
             size[3]= fileSize;

             Serial.write(size, 4);
             break;
          
          case 0x07: 
            first_chunk_size_byte = Serial.read();
            second_chunk_size_byte = Serial.read();
            set_chunk_size(first_chunk_size_byte, second_chunk_size_byte);
            free_serial_buffer();
            Send_update(file_to_be_send);  
            break;   
   
        }
     }
     free_serial_buffer();
     yield();
     delay(1000);
}

void free_serial_buffer()
{
  uint8_t no_of_bytes = Serial.available();
  for(uint8_t iterator = 0; iterator < no_of_bytes; iterator++)
  {
    Serial.read();
  }
}

boolean check_pckg_version()
{
  String pckg_Version;

  //getting pckg_Version from firebase
  Firebase.RTDB.getString(&fbdo, F("/pckg_version") , &pckg_Version );
  Serial1.println(pckg_Version);

  //convert string to array of chars
	char pckg_Version_chars[pckg_Version.length() + 1];
	strcpy(pckg_Version_chars, pckg_Version.c_str());

  // Extract the major token
  char * token = strtok(pckg_Version_chars, ".");
  pckg_major=atoi(token);

  // Extract the minor token
  token = strtok(NULL, ".");
  pckg_minor=atoi(token);

  // Extract the path token
  token = strtok(NULL, ".");
  pckg_patch=atoi(token);

  if(ECUs_major > pckg_major)
  {
    return false;  
  }
  else if(ECUs_major < pckg_major)
  {
    get_attributes();
    return true;  
  }
            
  if(ECUs_minor > pckg_minor)
  {
    return false;  
  }
  else if(ECUs_minor < pckg_minor)
  {
    get_attributes();
    return true;  
  }

  if(ECUs_patch > pckg_patch)
  {
    return false;  
  }
  else if(ECUs_patch < pckg_patch)
  {
    get_attributes();
    return true;  
  }
  return false;
}


//function to get attributes needed from RTDB on firebase cloud
void get_attributes()
{
  //getting targeted_ecus_21m from firebase
  Firebase.RTDB.getInt(&fbdo, F("/targeted_ecus_21m") , &targeted_ecus_21m );
  Serial1.println(targeted_ecus_21m);

  //getting is_urgent from firebase
  Firebase.RTDB.getInt(&fbdo, F("/is_urgent") , &is_urgent );
  Serial1.println(is_urgent);


  //getting crc struct for master from firebase
  Firebase.RTDB.getInt(&fbdo, F("/CRC/Master ECU/image") , &master_crc_image );
  Serial1.println(master_crc_image);
  master_crc.image=master_crc_image;
  Firebase.RTDB.getInt(&fbdo, F("/CRC/Master ECU/update_data") , &master_crc_update_data );
  master_crc.update_data=master_crc_update_data;
  Serial1.println(master_crc.update_data);

  //getting crc struct for target1 from firebase
  Firebase.RTDB.getInt(&fbdo, F("/CRC/Target 1/image") , &target1_crc_image );
  Serial1.println(target1_crc_image);
  
  target1_crc.image=target1_crc_image;
  Firebase.RTDB.getInt(&fbdo, F("/CRC/Target 1/update_data") , &target1_crc_update_data );
  Serial1.println(target1_crc_update_data);
  target1_crc.update_data=target1_crc_update_data;

  //getting crc struct for target2 from firebase
  Firebase.RTDB.getInt(&fbdo, F("/CRC/Target 2/image") , &target2_crc_image );
  Serial1.println(target2_crc_image);
  target2_crc.image=target2_crc_image;
  Firebase.RTDB.getInt(&fbdo, F("/CRC/Target 2/update_data") , &target2_crc_update_data );
  Serial1.println(target2_crc_update_data);
  target2_crc.update_data=target2_crc_update_data;
}


//whole system initialization
void init_all(){
  Serial.begin(UART_BAUDRATE);
  WIFI_Connect();
  FIREBASE_init();
  FILESYSTEM_init();
}

size_t get_file_size(target_id id, file_type type)
{
  Serial1.println("calculating file size...");

  SPIFFS.begin();

  size_t filesize;

  File file;

  if(type == bin)
  {
    file = SPIFFS.open( "/"+ECUs_paths[id].bin, "r");
    file_to_be_send = ECUs_paths[id].bin;
  }
  else if(type == data)
  {
    file = SPIFFS.open( "/"+ECUs_paths[id].data, "r");
    file_to_be_send = ECUs_paths[id].data;
  } 
  
  if(file.available()) 
  {
    filesize = file.size();
    Serial1.println(filesize);
  }
  file.close();
  SPIFFS.end();
  
  Serial1.println("file size is calculated");
  return filesize;
}

void set_chunk_size(uint16_t first_byte, uint8_t second_byte)
{
  chunk_size = chunk_size | (first_byte<<8);
  chunk_size = chunk_size | second_byte;
}
  

boolean Download_Files() {
  while(1)
  {
    if (Firebase.ready() && !taskCompleted) 
    {
      taskCompleted = true;

      Serial1.println("Download file...");
      
      if(targeted_ecus_21m & 1)
      {
        if ((!(Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,master_paths.data  /* path of remote file stored in the bucket */,master_paths.data /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))) 
            || (!(Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,master_paths.bin  /* path of remote file stored in the bucket */,master_paths.bin /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))))
        {
          Serial1.println(fbdo.errorReason());
          return false;
        }
      }

      if(((targeted_ecus_21m >> 1) & 1))
      {
        if ((!(Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,target1_paths.data  /* path of remote file stored in the bucket */,target1_paths.data /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))) 
            || (!(Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,target1_paths.bin  /* path of remote file stored in the bucket */,target1_paths.bin /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))))
        {
          Serial1.println(fbdo.errorReason());
          return false;
        }
      }

      if(((targeted_ecus_21m >> 2) & 1))
      {
        if ((!(Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,target2_paths.data  /* path of remote file stored in the bucket */,target2_paths.data /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))) 
            || (!(Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,target2_paths.bin  /* path of remote file stored in the bucket */,target2_paths.bin /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))))
        {
          Serial1.println(fbdo.errorReason());
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
    Serial1.println("sending file...");

    SPIFFS.begin();

    File file = SPIFFS.open( "/"+path , "r");

    static int count = 0;
  
    while (file.available()) 
    {
      char buf;
    
      file.readBytes(&buf, sizeof(buf));
      Serial.write(buf);

      count++;

      if(count == chunk_size)
      {
        while(1)
        {
          if(Serial.available() > 0)
          {
            if(Serial.read() == 0x00)
            {
              break;
            }
          }
          yield();
        }
        count = 0;
      }
      free_serial_buffer();
      yield();
    }
    count = 0;
    file.close();
    SPIFFS.end();
}

void FILESYSTEM_init()
{
    SPIFFS.begin();

    SPIFFS.mkdir("/OEM");

    SPIFFS.mkdir("/OEM/MasterECU");
    SPIFFS.mkdir("/OEM/MasterECU/Secure");
    SPIFFS.mkdir("/OEM/MasterECU/Binary");

    SPIFFS.mkdir("/OEM/Target 1");
    SPIFFS.mkdir("/OEM/Target 1/Secure");
    SPIFFS.mkdir("/OEM/Target 1/Binary");

    SPIFFS.mkdir("/OEM/Target 2");
    SPIFFS.mkdir("/OEM/Target 2/Secure");
    SPIFFS.mkdir("/OEM/Target 2/Binary");

    SPIFFS.end();
}

//Wifi initialization Function
void WIFI_Connect() 
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  #if _DEBUG_
    Serial1.print("\nConnecting to Wi-Fi");
  #endif
  char counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #if _DEBUG_
      Serial1.print(".");
    #endif
    counter++;
    if (counter == 15) {
      break;
    }
  }
  switch (WiFi.status()) {
  case WL_CONNECTED:
    #if _DEBUG_    
      Serial1.println("WiFi connected");
      Serial1.println(WiFi.SSID());
      Serial1.println(WiFi.localIP());
    #endif
    break;
  case WL_NO_SSID_AVAIL:
    #if _DEBUG_
      Serial1.println("SSID cannot be reached !");
    #endif
    ESP.restart();
    break;
  case WL_CONNECT_FAILED:
    #if _DEBUG_
      Serial1.println("Wrong Password !");
    #endif
    ESP.restart();
    break;
  default:
    #if _DEBUG_      
      Serial1.println("Unknown error !");
    #endif
    ESP.restart();
  }
}

//firebase initialization function
void FIREBASE_init() {
  #if _DEBUG_
    Serial1.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);
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
      Serial1.printf("Downloading file %s (%d) to %s\n", info.remoteFileName.c_str(), info.fileSize, info.localFileName.c_str());
    } else if (info.status == fb_esp_fcs_download_status_download) {
      Serial1.printf("Downloaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
    } else if (info.status == fb_esp_fcs_download_status_complete) {
      Serial1.println("Download completed\n");
    } else if (info.status == fb_esp_fcs_download_status_error) {
      Serial1.printf("Download failed, %s\n", info.errorMsg.c_str());
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
