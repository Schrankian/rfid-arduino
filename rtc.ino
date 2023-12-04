ThreeWire myWire(4, 5, 2);  // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

#define countof(a) (sizeof(a) / sizeof(a[0]))

String dateTimeToString(const RtcDateTime& dt) {
  char datestring[17];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u.%02u.%04u %02u:%02u"),
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

void rtcSetup() {
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (!Rtc.IsDateTimeValid()) {
    // Something went wrong
    Rtc.SetDateTime(compiled);
  }
  if (Rtc.GetIsWriteProtected()) {
    Rtc.SetIsWriteProtected(false);
  }
  if (!Rtc.GetIsRunning()) {
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    // Rtc is somehow older than the compile time -> Set the Rtc to the compile time
    Rtc.SetDateTime(compiled);
  } 
}
