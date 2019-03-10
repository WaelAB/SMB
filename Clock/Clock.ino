#include <LiquidCrystal.h>
#include <DS1302.h>

// Init the DS1302
// digital pins =RST, DAT, CLK
DS1302 rtc(5, 6, 7);


void setup()
{
  Serial.begin(9600);
  /*This block of code is supposed to run for the first time only to set the time and can be discarded
    until you want to set it up again
    rtc.halt(false); // Set the clock to run-mode, and disable the write protection
    rtc.writeProtect(false);

    // Setup LCD to 16x2 characters

    // The following lines can be commented out to use the values already stored in the DS1302
    rtc.setDOW(SUNDAY);        // Set Day-of-Week to FRIDAY
    rtc.setTime(22, 53, 0);     // Set the time to 12:00:00 (24hr format)
    rtc.setDate(10, 3, 2019);   // Set the date to Day, montth, year*/
}

void loop()
{
  // Display time centered on the upper line

  Serial.println(rtc.getTimeStr());

  delay (2000);
}
