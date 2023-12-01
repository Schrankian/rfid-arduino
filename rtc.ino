ThreeWire myWire(4, 5, 2);  // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

#define countof(a) (sizeof(a) / sizeof(a[0]))

String dateTimeToString(const RtcDateTime& dt) {
  char datestring[16];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u"),
             dt.Day(),
             dt.Month(),
             dt.Year(),
             dt.Hour(),
             dt.Minute());

  return String(datestring);
}

String currentDateTimeToString() {
  RtcDateTime now = Rtc.GetDateTime();

  if (!now.IsValid()) {
    // Something went wrong
    return "No Time";
  }
  return dateTimeToString(now);
}

bool isWeekend() {
  RtcDateTime now = Rtc.GetDateTime();

  //sunday->0 | Monday->1 | Tuesday->2 ...
  if(now.DayOfWeek() == 0 || now.DayOfWeek() == 6){
    return true;
  }else{
    return false;
  }
}

//--------------------Begin--------------------------
void rtcSetup() {
  //Serial.print(__DATE__);
  //Serial.println(__TIME__);
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  //Serial.print(dateTimeToString(compiled));
  //Serial.println();

  if (!Rtc.IsDateTimeValid()) {
    // Something went wrong
    //Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected()) {
    //Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning()) {
    //Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    //Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  } else if (now > compiled) {
    //Serial.println("RTC is newer than compile time. (this is expected)");
  } else if (now == compiled) {
    //Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
}
