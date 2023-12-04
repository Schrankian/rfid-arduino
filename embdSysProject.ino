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
String idleTemplate[2] = { "Warten auf", "RFID-Chip..." };
String registerModeTemplate[2] = { "Registriermodus", "RFID-Ger\xE1t +/-" };
String accessTemplate[2] = { "Zugang gew\xE1hrt", "-TIME-" };
String blockTemplate[2] = { "Zugang gesperrt", "-REASON-" };
String notRegisteredString = "Nicht angemeldet";
String isWeekendString = "Es ist Sa/So";
String addPrefixString = "+ ";
String remPrefixString = "- ";

void setup() {
  pinMode(6, OUTPUT); // Relais
  pinMode(7, OUTPUT); // LED
  rfidSetup();
  rtcSetup();
  displaySetup();
  changeDisplayValue(PERM, idleTemplate);
}

void loop() {
  rfidLoop();
  displayLoop();
}
