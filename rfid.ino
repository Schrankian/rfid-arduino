#define SS_PIN 10  // SDA to Pin 10
#define RST_PIN 9  // RST to Pin 9

#define MASTER_ID 457290

// create rfid receiver
MFRC522 mfrc522(SS_PIN, RST_PIN);

// States
bool registerMode = false;
long registeredIds[20];
size_t registeredIdCount = 0;

// Delay. Set by Interrupt
bool onCoolDown = false;

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

//--------------------Begin--------------------------
void rfidSetup() {
  // Initialize spi and rfid-receiver
  SPI.begin();
  mfrc522.PCD_Init();
}

void rfidLoop() {
  // Wait till Cooldown is over
  if (onCoolDown) {
    // Disable LED
    digitalWrite(7, LOW);
    return;
  }

  // Disable Relais (Default)
  digitalWrite(6, LOW);
  // Enable LED (Default)
  digitalWrite(7, HIGH);

  // Skip execution if no card is present
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Skip execution if no rfid-sender is present
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Read card id
  long code = 0;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    code = ((code + mfrc522.uid.uidByte[i]) * 10);
  }

  // Check for a valid id
  if (!registerMode && valueInArray(code, registeredIds, registeredIdCount)) {
    // Id is registered
    if (isWeekend()) {
      // Block on weekend -> Do nothing, just change display
      blockTemplate[1] = "It is weekend";
      changeDisplayValue(TEMP, blockTemplate);
    } else {
      // Enable Relais, change display
      accessTemplate[1] = currentDateTimeToString();
      changeDisplayValue(TEMP, accessTemplate);
      digitalWrite(6, HIGH);
    }

  } else if (code != MASTER_ID && !registerMode) {
    // Id not registered -> Do nothing, just change display
    blockTemplate[1] = "Not registered";
    changeDisplayValue(TEMP, blockTemplate);
  }

  // Register mode
  if (registerMode && code != MASTER_ID) {
    if (valueInArray(code, registeredIds, registeredIdCount)) {
      // Registered Card -> Remove
      removeValue(code, registeredIds, registeredIdCount);
      registeredIdCount--;
      // Print the removed id
      String deletedIdString[2] = { registerModeTemplate[0], "Del ID: " + String(code) };
      changeDisplayValue(TEMP, deletedIdString);
    } else {
      // New Card -> Add
      registeredIds[registeredIdCount] = code;
      registeredIdCount++;
      // Print the new registered id
      String registeredIdString[2] = { registerModeTemplate[0], "New ID: " + String(code) };
      changeDisplayValue(TEMP, registeredIdString);
    }
  }

  // Toggle register mode
  if (code == MASTER_ID) {
    if (!registerMode) {
      registerMode = true;
      changeDisplayValue(PERM, registerModeTemplate);
    } else {
      registerMode = false;
      changeDisplayValue(PERM, idleTemplate);
    }
  }

  // Start delay -> Is later set to false by Interupt
  onCoolDown = true;
  // Controlls the amount of seconds, all new cards are blocked
  // Also controlls the timespan, the temporary display values are shown
  startTimer(1);
}