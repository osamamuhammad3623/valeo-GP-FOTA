
#if 0
  //get all files in FileSystem
  #include "LittleFS.h"
 
  void listAllFilesInDir(String dir_path)
  {
    Dir dir = LittleFS.openDir(dir_path);
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
  void setup() {
  
    Serial.begin(115200);
    if (LittleFS.begin()) {
      listAllFilesInDir("/");
    }
  }
 
  void loop() {}

#endif