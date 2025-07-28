#include <Wire.h>
#include <Adafruit_PN532.h>
#include <Arduino.h>

#define SDA_PIN 8
#define SCL_PIN 9

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup() {
  Serial.begin(115200);
  delay(3000);
  
  Serial.println("=== ESP32-C3 NFC Reader Starting ===");
  Serial.print("SDA Pin: "); Serial.println(SDA_PIN);
  Serial.print("SCL Pin: "); Serial.println(SCL_PIN);
  
  // Enable pullups
  pinMode(SDA_PIN, INPUT_PULLUP);
  pinMode(SCL_PIN, INPUT_PULLUP);
  
  Serial.println("Initializing PN532...");
  nfc.begin();

  Serial.println("Getting firmware version...");
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("ERROR: Didn't find PN532 - check wiring!");
    while (1) {
      delay(1000);
      Serial.println("Still waiting for PN532...");
    }
  }

  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print("."); Serial.println((versiondata>>8) & 0xFF, DEC);

  nfc.SAMConfig();
  Serial.println("PN532 configured successfully!");
  Serial.println("Waiting for an NFC card...");
  Serial.println("===========================");
}

void loop() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    Serial.println("\n*** CARD DETECTED ***");
    Serial.print("Found card with UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      if (uid[i] < 0x10) Serial.print("0");
      Serial.print(uid[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    Serial.print("UID Length: "); Serial.print(uidLength); Serial.println(" bytes");
    Serial.println("*** END CARD INFO ***\n");
    
    delay(1000); // Delay before reading again
  } else {
    // Optional: show scanning status
    Serial.print(".");
    delay(100);
  }
}