// The main Project file for the embedded systems project.
// Calls functions from other files in the same folder.
// The compiler combines theses, as if they where on file.

//RTC
#include <ThreeWire.h>
#include <RtcDS1302.h>
//RFID
#include <SPI.h>
#include <MFRC522.h>
//Display
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

enum DisplayType {
  PERM,
  TEMP
};

// Templates
String idleTemplate[2] = { "Waiting for", "Connections..." };
String registerModeTemplate[2] = { "Register Mode", "Scan new Id" };
String accessTemplate[2] = { "Access Granted", "-TIME-" };
String blockTemplate[2] = { "Access Blocked", "-REASON-" };

void setup() {
  //Serial.begin(9600);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  rfidSetup();
  rtcSetup();
  displaySetup();
  changeDisplayValue(PERM, idleTemplate);
}

void loop() {
  rfidLoop();
  displayLoop();
}
