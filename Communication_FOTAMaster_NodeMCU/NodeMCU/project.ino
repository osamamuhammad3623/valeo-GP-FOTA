#define ESP8266
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include "FS.h"
#include <Firebase_ESP_Client.h>
// Provide the token generation process info.
#include <addons/TokenHelper.h>
// Provide the SD card interfaces setting and mounting
#include <addons/SDHelper.h>
#include "config.h"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool taskCompleted = false;
String filename="/bla_bla.txt";

void setup()
{
  
    Serial.begin(115200);
    
    WIFI_Connect();
    FIREBASE_init();  
    Firebase.ready() ; //should be called repeatedly to handle authentication tasks.
    //String url_image;
    //Firebase.RTDB.getString(&fbdo, F("Url/"));
    // url_image=fbdo.to<const char *>();
    //Serial.println(url_image);
    if (Firebase.ready() && !taskCompleted){
        taskCompleted = true;
        Serial.println("\nDownload file...\n");
        // The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
        if (!Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, "V1_0_0/application.bin" /* path of remote file stored in the bucket */, "bla_bla.txt" /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */))
            Serial.println(fbdo.errorReason());
    }
{
 

  
}
}


void loop()
{

  // if (SPIFFS.begin()) {
  //       listAllFilesInDir("/");
  //     }
  uint8_t flag =0x05;
  Serial.println("Hello World");
  while(Serial.available() <= 0){}
  // Serial.println("2222222222");
  while(Serial.read() !=flag ){}
  //Serial.println("3333333333");
  SPIFFS.begin();

  File file = SPIFFS.open(filename, "r");
  if (!file) {
    Serial.print("not available path______________________");
  }

  while (file.available()) {
    //char buf[32]={0};
    char buf;

    size_t bytesRead = file.readBytes(&buf, sizeof(buf));
    //buf[bytesRead] = '\0';
    Serial.print(buf);
    //delay(100);
    yield();
  }

  file.close();
  SPIFFS.end();
  //delay(3000);
}


void WIFI_Connect(){
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    char counter=0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      counter++;
      if(counter==15){
        break;
      }
    }
    switch (WiFi.status()) {
      case WL_CONNECTED:
        Serial.println("\nWiFi connected");
        Serial.println(WiFi.SSID());
        Serial.println(WiFi.localIP());
        break;
      case WL_NO_SSID_AVAIL:
        Serial.println("\nSSID cannot be reached !"); 
        ESP.restart();       
        break; 
      case WL_CONNECT_FAILED:
        Serial.println("\nWrong Password !");  
        ESP.restart();      
        break; 
      default:
        Serial.println("\nUnknown error !"); 
        delay(2000);
        ESP.restart();
    }  
}

void FIREBASE_init(){
      Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;
    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

   /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

   /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  #if defined(ESP8266)
      // required for large file data, increase Rx size as needed.
      fbdo.setBSSLBufferSize(1024 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
  #endif

    /* Assign download buffer size in byte */
    // Data to be downloaded will read as multiple chunks with this size, to compromise between speed and memory used for buffering.
    // The memory from external SRAM/PSRAM will not use in the TCP client internal rx buffer.
    config.fcs.download_buffer_size = 2048;

    Firebase.begin(&config, &auth);

    Firebase.reconnectWiFi(true);

    // if use SD card, mount it.
    //SD_Card_Mounting(); // See src/addons/SDHelper.h
}

// The Firebase Storage download callback function
void fcsDownloadCallback(FCS_DownloadStatusInfo info)
{
    if (info.status == fb_esp_fcs_download_status_init)
    {
        Serial.printf("Downloading file %s (%d) to %s\n", info.remoteFileName.c_str(), info.fileSize, info.localFileName.c_str());
    }
    else if (info.status == fb_esp_fcs_download_status_download)
    {
        Serial.printf("Downloaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
    }
    else if (info.status == fb_esp_fcs_download_status_complete)
    {
        Serial.println("Download completed\n");
    }
    else if (info.status == fb_esp_fcs_download_status_error)
    {
        Serial.printf("Download failed, %s\n", info.errorMsg.c_str());
    }
}
  //get all files in FileSystem
void listAllFilesInDir(String dir_path)
  {
    Dir dir = SPIFFS.openDir(dir_path);
    while(dir.next()) {
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