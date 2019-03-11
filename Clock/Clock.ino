#include <LiquidCrystal_I2C.h>
#include <DS1302.h>

// Init the DS1302
// digital pins =RST, DAT, CLK
DS1302 rtc(5, 6, 7);
LiquidCrystal_I2C lcd(0x27, 20, 4);

int buzzer = 4 ;
void setup()
{
  Serial.begin(9600);
  pinMode (buzzer, OUTPUT) ;

  //This block of code is supposed to run for the first time only to set the time and can be discarded
  //    until you want to set it up again
  rtc.halt(false); // Set the clock to run-mode, and disable the write protection
  rtc.writeProtect(false);
  // The following lines can be commented out to use the values already stored in the DS1302
  rtc.setTime(18, 59, 0);   // Set the time to 12:00:00 (24hr format)
  
  lcd.init(); // Start the LCD
  lcd.backlight(); // Trun the light of the screen
}

int alarm = 19;// example alarm time
String temp ; //temporary value
int intTime = 0; 
String hTime ;

void loop() {
  // Display time centered on the upper line
  lcd.clear();// Clear the screen from everything
  lcd.setCursor(0, 0);
  lcd.print(rtc.getTimeStr());

  temp = rtc.getTimeStr();// save time into temp value

  for (int i = 0 ; i < 2 ; i++) {
    hTime += temp[i]; // move only hours and save it into hTime
  }

  intTime = hTime.toInt(); // convert string time into integer
  if (intTime == alarm) { // if the time matches the alarm time
    AlarmTone(); // call AlarmTone method
  }
  hTime = "";
  delay (2000);
}


void AlarmTone() { //this method will activate the buzzer and play a tone
  while (true) {
    tone(buzzer, 1000);
    delay(500);
    tone(buzzer, 1500);
    delay(500);
    tone(buzzer, 2000);
    delay(500);
  }
}
