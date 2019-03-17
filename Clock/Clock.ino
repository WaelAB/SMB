#include <Wire.h> //using I2C
#include "RTClib.h" // include the clock libarary
#include <LiquidCrystal_I2C.h> // for the screen

LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS1307 rtc;

void setup () {
  Serial.begin(9600);

  lcd.init(); // Start the LCD
  lcd.backlight(); // Trun the light of the screen

  if (! rtc.begin()) {// Error  handling
    Serial.println("Couldn't find RTC");
    while (1);
  }
  // These two line of code is supposed to run for the first time only to set the time and can be commented
  // until you want to set it up again
  // DateTime now = rtc.now(); // Creating object of Date Time
  // rtc.adjust(now.unixtime()-10); // Adjusting time according to OS time and -10 because time was ahead by 10 seconds
}
int buzzer = 4 ;
String HourStr;
String MinStr;
int HourInt = 0;
int MinInt = 0;
int AlarmHr = 2;
int AlarmMin = 00;
void loop () {

  DateTime now = rtc.now(); // Creating object of DateTime

  HourStr = String(now.hour(), DEC); // Getting Hours is save into String
  MinStr = String(now.minute(), DEC);// Getting Hours is save into String

  Serial.print(HourStr);
  Serial.print(":");
  Serial.print(MinStr);
  Serial.println();


  HourInt = HourStr.toInt(); // convert string time into integer
  MinInt = MinStr.toInt();
  if (HourInt == AlarmHr && MinInt == AlarmMin) { // if the time matches the alarm time
    AlarmTone(); // call AlarmTone method

    lcd.clear();// Clear the screen from everything
    lcd.setCursor(0, 0);
    lcd.print(HourStr);
    lcd.print(':');
    lcd.print(MinStr);
    lcd.print(':');
    lcd.print(now.second(), DEC);
  }

  lcd.clear();// Clear the screen from everything
  lcd.setCursor(0, 0);
  lcd.print(HourStr);
  lcd.print(':');
  lcd.print(MinStr);
  lcd.print(':');
  lcd.print(now.second(), DEC);

  delay(1000);

}




void AlarmTone() { //this method will activate the buzzer and play a tone

  tone(buzzer, 1000);
  delay(500);
  tone(buzzer, 1500);
  delay(500);
  tone(buzzer, 2000);
  delay(500);
  noTone(buzzer);
}
