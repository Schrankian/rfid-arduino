
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set display type: 16 characters on 2 rows and address 0x27

String* lastState;

bool cooldownFinished = false;

void changeDisplayValue(DisplayType type, String value[2]) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(value[0]);
  lcd.setCursor(0, 1);
  lcd.print(value[1]);

  if (type == PERM) {
    lastState = value;
  }
}

//--------------------Begin--------------------------
void displaySetup() {
  // Initialize Screen
  lcd.init();
  // Enable background lighting
  lcd.backlight();
}

void displayLoop() {
  // Variable set by Interrupt
  if (!cooldownFinished) {
    return;
  }
  changeDisplayValue(PERM, lastState);
  cooldownFinished = false;
}