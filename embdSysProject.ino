// The main Project file for the embedded systems project.
// Calls functions from other files in the same folder.
// The compiler combines theses, as if they where on file.

//RTC
#include <ThreeWire.h>  
#include <RtcDS1302.h>
//RFID
#include <SPI.h> // SPI-Bibiothek hinzufügen
#include <MFRC522.h> // RFID-Bibiothek hinzufügen
//Display
#include <Wire.h> // Wire Bibliothek einbinden
#include <LiquidCrystal_I2C.h> // Vorher hinzugefügte LiquidCrystal_I2C Bibliothek einbinden


void setup() {
  Serial.begin(9600);
  
  rfidSetup();
  rtcSetup();
  displaySetup();
}

void loop() {
  rfidLoop();
  rtcLoop();
  displayLoop();
}
