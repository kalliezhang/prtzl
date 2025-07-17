#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <Arduino.h>

#define SDA_PIN 8
#define SCL_PIN 9

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);  // Use GPIO 8 and 9 here

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Didn't find PN532");
    while (1);
  }

  nfc.SAMConfig();
  Serial.println("Waiting for an NFC card...");
}

void loop() {
  uint8_t success;
  uint8_t uid[7];
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.print("Found card with UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    delay(1000);
  }
}
