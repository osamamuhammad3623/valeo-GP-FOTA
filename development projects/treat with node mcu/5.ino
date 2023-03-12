
#if 1
  #include <FirebaseArduino.h>
  #include "ESP8266WiFi.h"

  #define FIREBASE_HOST "test-fb83e-default-rtdb.firebaseio.com"
  #define FIREBASE_AUTH "mMSPNO4Fq0V58F7AjfPBJhyQIMvnffzxK52pSJ8i"

  // #define WIFI_SSID "TEData_8C0E7F"
  // #define WIFI_PASSWORD "adAm_@362B276 @"

  #define WIFI_SSID "madina77"
  #define WIFI_PASSWORD "madinatalbaasunet"

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
    Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  }
  
  int x=0;

  void loop(){

    Firebase.setString("Name","Abdelrahman Elsayed");
      // handle error
    if (Firebase.failed()) {
        Serial.print("firebase failed:");
        Serial.println(Firebase.error());  
        return;
    }
    delay(10000);  

    Firebase.setInt("Variable/value1",x++);
      // handle error
    if (Firebase.failed()) {
        Serial.print("firebase failed:");
        Serial.println(Firebase.error());  
        return;
    }
    delay(10000); 
    Firebase.setInt("Variable/value2",x++);
      // handle error
    if (Firebase.failed()) {
        Serial.print("firebase failed:");
        Serial.println(Firebase.error());  
        return;
    }
    delay(10000);  
    

  }

#endif