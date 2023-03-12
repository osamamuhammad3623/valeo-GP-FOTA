#if 0
  #include <ESP8266WiFi.h>

  const char ssid[]="TEData_8C0E7F";
  const char pass[]="adAm_@362B276 @";

  void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    WiFi.begin(ssid,pass);
    while(WiFi.status() !=WL_CONNECTED){
      delay(500);
      Serial.print("not connected yet\n");
      WiFi.begin(ssid,pass);
      //Serial.print(WiFi.status());
    }
    Serial.print("wifi connected\n");

  }
  void loop() {
    // put your main code here, to run repeatedly:

  }
#endif