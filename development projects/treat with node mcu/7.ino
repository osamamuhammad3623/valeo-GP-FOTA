
#if 0
  #include "LittleFS.h"
  void setup(){
    LittleFS.begin();
    Serial.begin(115200);
    Serial.println("0");
    if(LittleFS.exists("/C:/Users/Lenovo/Desktop/abc.txt")){
      Serial.println("1");
        LittleFS.remove("/C:/Users/Lenovo/Desktop/abc.txt");
    }
    if(LittleFS.exists("/abc.txt")){
      Serial.println("2");
        LittleFS.remove("/abc.txt");
    }
    if(LittleFS.exists("/abdo.txt")){
      Serial.println("3");
        LittleFS.remove("/abdo.txt");
    }

  if(LittleFS.exists("/example.txt")){
    Serial.println("4");
        LittleFS.remove("/example.txt");
    }
  LittleFS.end();
  }
  void loop(){

  }
#endif