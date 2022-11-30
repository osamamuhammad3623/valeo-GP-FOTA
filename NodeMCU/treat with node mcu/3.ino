#if 0
  #include <ESP8266WiFi.h>
  const char* ssid = "TEData_8C0E7F";
  const char* password = "adAm_@362B276 @";
  
  void setup() {
    Serial.begin(115200);
    delay(10);
  
    Serial.print("\nTry Connecting to ");
    Serial.println(ssid);
  
    WiFi.begin(ssid, password);
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
        delay(2000);
        ESP.restart();
    }

  }
  
  void loop() {

  }
#endif
