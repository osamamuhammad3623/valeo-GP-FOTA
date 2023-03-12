
#if 0

  #include <FirebaseArduino.h>
  #include "ESP8266WiFi.h"
  #include <ESP8266HTTPClient.h>
  #include <ESP8266WiFiMulti.h>
  #include <LittleFS.h>

  #define FIREBASE_HOST "test-fb83e-default-rtdb.firebaseio.com"
  #define FIREBASE_AUTH "mMSPNO4Fq0V58F7AjfPBJhyQIMvnffzxK52pSJ8i"

  #define WIFI_SSID "TEData_8C0E7F"
  #define WIFI_PASSWORD "adAm_@362B276 @"

  //#define FILE_URL "https://firebasestorage.googleapis.com/v0/b/test-fb83e.appspot.com/o/abc.txt?alt=media&token=9f21b90c-87e3-4e9c-a0d4-90d76ccb806e"
  //#define FILE_URL "http://100daysofcode.s3-website-eu-west-1.amazonaws.com/schedule.txt"
  #define FILE_URL "gs://test-fb83e.appspot.com/abc.txt"
  
  
  #define filename_FS "/test.txt"  
  
  void ff(){
    WiFiClient wificlient;
    HTTPClient httpclient;
    // if(!httpclient.begin(wificlient,FILE_URL)){
    //   Serial.println("could not connect to the internet");
    // }
    if (!httpclient.begin(wificlient,FILE_URL )) {
      Serial.println("could not connect to the internet");
      return;
    }
    int statuscode=httpclient.GET();

    if(statuscode < 0){
      Serial.printf("GET failed, error: %s \n", httpclient.errorToString(statuscode).c_str());
      return;
    }
    if(statuscode != HTTP_CODE_OK){
      Serial.printf("invalid status code: %d\n",statuscode);
      return ;
    }

    auto body =httpclient.getString();
    //Serial.println(body);

    LittleFS.begin();
    File file = LittleFS.open(filename_FS,"w");
    auto bytesWritten = file.write(body.c_str());

    if (bytesWritten == 0) {
      Serial.println("could not write to the file");
      return ;
    }
    file.close();
    LittleFS.end();

  }
  void setup(){
    
    Serial.begin(115200);

    WiFi.disconnect();
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
    Serial.print("\nTry Connecting to ");
    Serial.println(WIFI_SSID);
  
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
        break; 
      case WL_CONNECT_FAILED:
        Serial.println("\nWrong Password !");        
        break; 
      default:
        Serial.println("\nUnknown error !"); 
        delay(5000);
        ESP.restart();
    }
    //Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
    ff();
  }
  
  int x=0;

  void loop(){
    LittleFS.begin();

    File file = LittleFS.open(filename_FS, "r");

    if (!file) {
      // handle the error
    }

    auto content = file.readString();

    Serial.println(content.c_str());

    file.close();

    LittleFS.end();
    delay(5000);

  }
#endif

