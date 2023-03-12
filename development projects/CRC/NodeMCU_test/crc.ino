#define APP_SIZE 10 //6516
#define POLYNOMIAL 0x04C11DB7

uint8_t dataBuffer[APP_SIZE] = {0xCC, 0x11, 0x22, 0xAA, 0xBB, 0xDD, 0xEE, 0xFF, 0x33, 0x44};

uint32_t calculate_CRC32(uint8_t * data, uint16_t bytesNum, uint32_t polynom) {
   uint32_t crc = 0xFFFFFFFF;
   int remainder = bytesNum%4;
   int paddingBytes = 0;
   if (remainder) {
    paddingBytes = 4-remainder;
   }
   while (bytesNum--) {
    if ((bytesNum & 0xFF) == 0) yield();
    uint8_t dataByte = *data++;
    crc ^= ((uint32_t)dataByte) << 24;
    for (int i = 0; i<8; i++) {
      if (crc & (1UL<<31)) {
        crc <<= 1;
        crc ^= polynom;
      } else {
        crc <<= 1;
      }
    }
   }
   while (paddingBytes--) {
    for (int i = 0; i<8; i++) {
      if (crc & (1UL<<31)) {
        crc <<= 1;
        crc ^= polynom;
      } else {
        crc <<= 1;
      }
    }
   }
   return crc;   
}

void setup() {
  uint32_t crcResult = calculate_CRC32(dataBuffer, APP_SIZE, POLYNOMIAL);
  Serial.begin(9600);
  Serial.println(crcResult);
}

void loop() {

}
