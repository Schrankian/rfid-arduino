#define SS_PIN 10  // SDA to Pin 10
#define RST_PIN 9  // RST to Pin 9

#define MASTER_ID 457290

// create rfid receiver
MFRC522 mfrc522(SS_PIN, RST_PIN);

// States
bool registerMode = false;
long registeredIds[20];
size_t registeredIdCount = 0;

// Delay
unsigned long lastScanTime = 0;
unsigned long scanCooldown = 2000;

bool valueInArray(long val, long *arr, size_t n) {
  for (size_t i = 0; i < n; i++) {
    if (arr[i] == val)
      return true;
  }
  return false;
}

void removeValue(long val, long *arr, size_t n) {
  for (size_t i = 0; i < n; i++) {
    if (arr[i] == val) {
      for (size_t j = i; j < n - 1; j++) {
        arr[j] = arr[j + 1];
      }
      n--;
      i--;
    }
  }
}


void printArray(long *arr, size_t n) {
  for (size_t i = 0; i < n; i++) {
    Serial.println(arr[i]);
  }
}

//--------------------Begin--------------------------
void rfidSetup() {
  // Initialize spi and rfid-receiver
  SPI.begin();
  mfrc522.PCD_Init();
}

void rfidLoop() {
  // Soft delay
  if (millis() - lastScanTime < scanCooldown) {
    return;
  }

  // Skip execution if no card is present
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Skip execution if no rfid-sender is present
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  long code = 0;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    code = ((code + mfrc522.uid.uidByte[i]) * 10);
  }

  if (!registerMode && valueInArray(code, registeredIds, registeredIdCount)) {
    Serial.println("Access allowed");
    accessTemplate[1] = currentDateTimeToString();
    changeDisplayValue(TEMP, accessTemplate);
  } else if (code != 457290 && !registerMode) {
    Serial.println("Access denied");
    changeDisplayValue(TEMP, blockTemplate);
  }

  if (registerMode && code != 457290) {
    if (valueInArray(code, registeredIds, registeredIdCount)) {
      removeValue(code, registeredIds, registeredIdCount);
      registeredIdCount--;
      Serial.print("Card deleted with id: ");
      Serial.println(code);
      String deletedIdString[2] = { registerModeTemplate[0], "Del ID: " + String(code) };
      changeDisplayValue(TEMP, deletedIdString);
    } else {
      registeredIds[registeredIdCount] = code;
      registeredIdCount++;
      Serial.print("Card registered with id: ");
      Serial.println(code);
      String registeredIdString[2] = { registerModeTemplate[0], "New ID: " + String(code) };
      changeDisplayValue(TEMP, registeredIdString);
    }
  }

  if (code == MASTER_ID) {
    Serial.print("Register-mode is now: ");
    if (!registerMode) {
      registerMode = true;
      Serial.println("On");
      changeDisplayValue(PERM, registerModeTemplate);
    } else {
      registerMode = false;
      Serial.println("Off");
      changeDisplayValue(PERM, idleTemplate);
    }
  }

  //Serial.println("Die Kartennummer lautet: " + code);

  // Newline
  Serial.println();

  // Start delay
  lastScanTime = millis();
}