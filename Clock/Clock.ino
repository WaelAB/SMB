
#include <Wire.h> //using I2C
#include "RTClib.h" // include the clock libarary

#include <LiquidCrystal_I2C.h> // for the screen
LiquidCrystal_I2C lcd(0x27, 20, 4);

RTC_DS1307 rtc;



void setup () {
lcd.init(); // Start the LCD
  lcd.backlight(); // Trun the light of the screen


  Serial.begin(115200);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
 // DateTime now = rtc.now();
  //rtc.adjust(now.unixtime()-10);
}

void loop () {
    
    DateTime now = rtc.now();
    String hh;
    hh=String(now.hour(), DEC);// changing the time to String
    Serial.println ("my time");
    Serial.print(hh);
    Serial.print(":");
    String mm;
    mm = String(now.minute(), DEC);
    Serial.print(mm);
    
  
    lcd.clear();// Clear the screen from everything
    lcd.setCursor(0, 0);
    lcd.print(now.hour(), DEC);
    lcd.print(':');
     lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.println(now.second(), DEC);
    
    
    
    
    Serial.println();
    delay(1000);
}
