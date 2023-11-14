#define SS_PIN 10 // SDA an Pin 10 (bei MEGA anders)
#define RST_PIN 9 // RST an Pin 9 (bei MEGA anders)

MFRC522 mfrc522(SS_PIN, RST_PIN); // RFID-Empfänger benennen

// States
bool registerMode = false;
long lastId = 0;
long registeredIds[20];
size_t registeredIdCount = 0;

bool valueinarray(long val, long *arr, size_t n) {
    for(size_t i = 0; i < n; i++) {
        if(arr[i] == val)
            return true;
    }
    return false;
}

void printArray(long *arr, size_t n){
  for(size_t i = 0; i < n; i++) {
        Serial.println(arr[i]);
    }
}

//--------------------Begin--------------------------
void rfidSetup(){
  SPI.begin(); // SPI-Verbindung aufbauen
  mfrc522.PCD_Init(); // Initialisierung des RFID-Empfängers
}

void rfidLoop(){
    if (!mfrc522.PICC_IsNewCardPresent()){ // Wenn keine Karte in Reichweite ist...
    return; // ...springt das Programm zurück vor die if-Schleife, womit sich die Abfrage wiederholt.
  }

  if ( ! mfrc522.PICC_ReadCardSerial()){ // Wenn kein RFID-Sender ausgewählt wurde
    return; // ...springt das Programm zurück vor die if-Schleife, womit sich die Abfrage wiederholt.
  }

  long code=0;
  for (byte i = 0; i < mfrc522.uid.size; i++){
    code=((code+mfrc522.uid.uidByte[i])*10);
  }

  if(lastId == code){
    return;
  }
  lastId = code;

  if(!registerMode && valueinarray(code, registeredIds, registeredIdCount)){
    Serial.println("Access allowed");
  }else if(code != 457290 && !registerMode){
    Serial.println("Access denied");
  }

  if(registerMode && !valueinarray(code, registeredIds, registeredIdCount) && code != 457290){
    registeredIds[registeredIdCount] = code;
    registeredIdCount++;
    Serial.print("Card registered with id: ");
    Serial.println(code);
  }

  if(code == 457290){
    Serial.print("Register-mode is now: ");
    if(!registerMode){
      registerMode = true;
      Serial.println("On");
    }else{
      registerMode = false;
      Serial.println("Off");
    }
  }

  //Serial.println("Die Kartennummer lautet: " + code);

  Serial.println(); // Mit dieser Zeile wird auf dem Serial Monitor nur ein Zeilenumbruch gemacht.
}