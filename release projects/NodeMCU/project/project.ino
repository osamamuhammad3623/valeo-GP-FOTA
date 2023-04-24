#include "Config.h"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool taskCompleted = false;
String FS_filename = "app_send.bin";

//should be getted from RTDB
String Firebase_filename = "V1_0_0/application.bin";

void setup() {
	init_all();

	while(1){
		if (Firebase.ready() && !taskCompleted) {
			taskCompleted = true;
			#if _DEBUG_
				Serial.println("Download file...");
			#endif
			
			// The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
			if (!Firebase.Storage.download(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */,Firebase_filename  /* path of remote file stored in the bucket */,FS_filename /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, fcsDownloadCallback /* callback function */)){
				#if _DEBUG_
					Serial.println(fbdo.errorReason());
				#endif
			}
			break;
		}
		yield();
	}

}


void loop() {
  
	uint8_t flag = 0x03;
	// #if _DEBUG_
	// 	Serial.println("waiting for flag to start communication");
	// #endif
 
	if (Serial.available() >0) {
        // #if _DEBUG_
        //     Serial.println("recieve from uart");
        // #endif
        uint8_t readedflag=Serial.read();
        if (readedflag == flag) {
        
            LittleFS.begin();

            File file = LittleFS.open( "/"+FS_filename , "r");
            file.readBytes(char *buffer, size_t length);
            if (!file) {
				// #if _DEBUG_
				// 	Serial.print("not available path______________________");
				// #endif
            }

            while (file.available()) {
              char buf;
            
              size_t bytesRead = file.readBytes(&buf, sizeof(buf));
              Serial.write(buf);
              yield();
            }
            // #if _DEBUG_
            //   	Serial.println("end sending data");
            // #endif

            file.close();
            LittleFS.end();
        }
  }
 	yield();
 	delay(1000);
}

//whole system initialization
void init_all(){
	Serial.begin(UART_BAUDRATE);
	//#if _DEBUG_
	//	Serial.begin(UART_BAUDRATE);
	//#endif
	WIFI_Connect();
	FIREBASE_init();
}

//Wifi initialization Function
void WIFI_Connect() {
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

	/*
	to get all the files in the FS
		if (SPIFFS.begin()) {
			listAllFilesInDir("/");
		}
	*/
}
