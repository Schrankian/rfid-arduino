
LiquidCrystal_I2C lcd(0x27, 16, 2);  //Hier wird festgelegt um was für einen Display es sich handelt. In diesem Fall eines mit 16 Zeichen in 2 Zeilen und der HEX-Adresse 0x27. Für ein vierzeiliges I2C-LCD verwendet man den Code "LiquidCrystal_I2C lcd(0x27, 20, 4)"

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
  // Soft delay
  if (!cooldownFinished) {
    return;
  }
  changeDisplayValue(PERM, lastState);
  cooldownFinished = false;
}