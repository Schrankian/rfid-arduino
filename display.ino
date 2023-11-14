
LiquidCrystal_I2C lcd(0x27, 16, 2); //Hier wird festgelegt um was für einen Display es sich handelt. In diesem Fall eines mit 16 Zeichen in 2 Zeilen und der HEX-Adresse 0x27. Für ein vierzeiliges I2C-LCD verwendet man den Code "LiquidCrystal_I2C lcd(0x27, 20, 4)" 


String displayValue[2] = {"", ""};
AccessState currentState = IDLE;


void changeDisplayValue(AccessState state){
  switch(state){
    case IDLE:
      displayValue[0] = "Waiting for";
      displayValue[1] = "Connections...";
      break;
    case GRANTED:
      displayValue[0] = "Zugriff gewährt";
      displayValue[1] = "Bitte eintreten!";
      break;
    case BLOCKEDNORM:
      displayValue[0] = "Zugriff verweigert";
      displayValue[1] = "Bitte registriere dich!";
      break;
    case BLOCKEDTIME:
      displayValue[0] = "Zugriff verweigert";
      displayValue[1] = "Es ist Wochenende";
    case REGISTER:
      displayValue[0] = "Register Mode";
      displayValue[1] = "Scan new IDs";
      break;
    default:
      displayValue[0] = "Something went";
      displayValue[1] = "horribly wrong!";
      break;
  }
}

//--------------------Begin--------------------------
void displaySetup(){
  lcd.init(); //Im Setup wird der LCD gestartet 
  lcd.backlight(); //Hintergrundbeleuchtung einschalten (lcd.noBacklight(); schaltet die Beleuchtung aus).  
}

void displayLoop(){
  lcd.setCursor(0, 0);//Hier wird die Position des ersten Zeichens festgelegt. In diesem Fall bedeutet (0,0) das erste Zeichen in der ersten Zeile. 
  lcd.print(displayValue[0]); 
  lcd.setCursor(0, 1);// In diesem Fall bedeutet (0,1) das erste Zeichen in der zweiten Zeile. 
  lcd.print(displayValue[1]); 
}