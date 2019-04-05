#include "medicine.h" // medicine class
#include "user.h" // user class
#include <Wire.h> // i2c between arduino and esp
#include <LiquidCrystal_I2C.h> // LCD 
#include "HX711.h" // load cell and aplifire
#include "Cooler.h"// Cooler class
#include "RTClib.h" // include the clock libarary
//--------------
HX711 scale1(3, 2);// digital pins = DT ,SCK
HX711 scale2(6, 7);// digital pins = DT ,SCK
//--------------
RTC_DS1307 rtc;//create object from RTC module
LiquidCrystal_I2C lcd(0x27, 16, 2);//create object of the LCD
//--------------
int buzzer = 4; // digital pin of the buzzer
float calibration_factor = 2000; // this calibration factor is adjusted according to my load cell
float calibration_factor2 = 2500;
float units1; // where the weight is stored
float units2; // where the weight is stored
//--------------
medicine med[2];// create array of medicine type
User user[2]; // create array of medicine type
Cooler cool; // create object of cooler type
//--------------
String uName ; // User Name
int uID = 0; // User ID
int ID = 0;
String MedName; // Medicine Name
String Dosage; // Dosage amount
int DosageNum = 0 ; // The number of doses
boolean InStock; // status for medicine
int ConNum = 0 ; // Container Number
char c ; // Temporary varible to store chars
int strLen = 0; // Temporary varible to store string length
int DTime = 0; // Temporary to store dosage time and move it into time array
int DMin = 0; // Temporary to store dosage time and move it into time array
int medSize = 0 ;
//--------------
String HourStr;
int HourInt = 0;
String MinStr ;
int MinInt = 0 ;
String SecStr;
int SecInt = 0;
int tempMin = 0;
//--------------
float firstWeight = 0.0;
float CurrentWeight = 0.0;
float oldWeight = 0.0;
float outOfStock = 0.0;
float ssss = 0.0;
//--------------
int GotIt = -1 ;// Message to esp initated with -1
//--------------
int AlarmToneReturn=0;
int LedReturn=0;
void setup() {

  Serial.begin(9600);// start serial for debug
  Wire.begin(8); // join i2c bus with address 8 to the esp
  //--------------
  lcd.init(); // Start the LCD
  lcd.backlight(); // Trun the light of the screen
  //--------------
  Wire.onReceive(receiveEvent); // register receive event from esp
  // Wire.onRequest();
  //--------------
  pinMode(cool.relayPin, OUTPUT);// Realy pin
  pinMode (buzzer, OUTPUT) ; // the buzzer pin should output the buzzer sound
  digitalWrite(cool.relayPin, LOW);// Realy pin that sould be low( powered off) by default
  //--------------
  if (! rtc.begin()) {// Error  handling
    Serial.println("Couldn't find RTC");
    while (1);
  }
  SettingUp();
}

void loop() {

  DateTime now = rtc.now(); // Creating object of DateTime
  cool.CheckTemp();
  HourStr = String(now.hour(), DEC); // Getting Hours and save it into String
  HourInt = HourStr.toInt(); // convert string hours into integer
  MinStr =  String(now.minute(), DEC); // Getting Minutes is save into String
  MinInt = MinStr.toInt();// convert string Minute into integer
  SecStr = String (now.second(), DEC);// Getting Seconds and save it into String
  SecInt = SecStr.toInt();// convert string Seconds into integer

  //Printing time on lcd
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  delay(1000);
  //lcd.clear();


  for ( int x = 0 ; x < medSize ; x++) {
    //    Serial.println("Hours");
    //    Serial.println(med[x].getTimes(0) );
    //    Serial.println(med[x].getTimes(1) );
    //    Serial.println(med[x].getTimes(2) );
    //
    //    Serial.println("Mins");
    //    Serial.println(med[x].getMinutes(0) );
    //    Serial.println(med[x].getMinutes(1) );
    //    Serial.println(med[x].getMinutes(2) );


    for ( int i = 0 ; i < med[x].getDosesNum(); i++) {
      if (med[x].getStockState() == true) {
        if (HourInt == med[x].getTimes(i) && MinInt ==  med[x].getMinutes(i) && SecInt <= 10) {

          if ( med[x].getContainerNum() == 0) {
            oldWeight = scale1.get_units(), 10;// weight before the alarm
            CurrentWeight = scale1.get_units(), 10;// weight after the user pick his medicine

            tempMin = MinInt;
            //            Serial.print("Temp");
            //            Serial.println(tempMin);
            while (true) {
              AlarmToneReturn=AlarmTone();
              digitalWrite(11,HIGH);
//              LedReturn=Led(11);
              //testAlarmLED(1,LedReturn,1,AlarmToneReturn);
              now = rtc.now();
              lcd.clear();
              lcd.setCursor(0, 0);// First digit , First line
              lcd.print("Please take");
              lcd.setCursor(0, 1);// First digit , Second line
              lcd.print(med[x].getDosage());

              delay(1000);

              CurrentWeight = scale1.get_units(), 10;// read the weight
              CurrentWeight = abs(CurrentWeight);
//              Serial.print ("Current");
//              Serial.println(CurrentWeight);

              delay (1000);
              if (oldWeight - CurrentWeight > 0.30 ) { // (&& CurrentWeight > 0.20 ).20 is threshold if we use a medicne packet
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("YOU HAVE TOOK ");  // .30 treshold for the diffrerance betweem each pill
                lcd.setCursor(0, 1);
                lcd.print("YOUR MEDICINE !");

                delay(1000);
                digitalWrite(11,LOW);
                //Wire.write(20);
                //Wire.write(true);
                //logSnd(HourInt);
                break;
              }

              MinStr =  String(now.minute(), DEC); // Getting Minutes is save into String
              MinInt = MinStr.toInt();
              Serial.println (MinInt);
              if (MinInt  - tempMin > 0) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("You missed ");
                lcd.setCursor(0, 1);
                lcd.print("your medicine");

                delay(1000);
                // update on FB
                break;
              }
            }

            //*********************
          } else if ( med[x].getContainerNum() == 1) {
            oldWeight = scale2.get_units(), 10;// weight before the alarm
            CurrentWeight = scale2.get_units(), 10;// weight after the user pick his medicine

            tempMin = MinInt;
            //            Serial.print("Temp");
            //            Serial.println(tempMin);
            while (true) {
              AlarmToneReturn=AlarmTone();
              LedReturn=Led(12);
            //  testAlarmLED(1,LedReturn,1,AlarmToneReturn);
              
            digitalWrite(11,HIGH);
              now = rtc.now();
              lcd.clear();
              lcd.setCursor(0, 0);// First digit , First line
              lcd.print("Please take ");
              lcd.setCursor(0, 1);// First digit , First line
              lcd.print(med[x].getDosage());

              delay(1000);

              CurrentWeight = scale2.get_units(), 10;// read the weight
              CurrentWeight = abs(CurrentWeight);
              //Serial.print ("Current");
              //Serial.println(CurrentWeight);
              delay (1000);

              if (oldWeight - CurrentWeight > 0.80) { // (&& CurrentWeight > 0.20 ).20 is threshold if we use a medicne packet
               lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("YOU HAVE TOOK ");  // .30 treshold for the diffrerance betweem each pill
                lcd.setCursor(0, 1);
                
                lcd.print("YOUR MEDICINE !");

                delay(1000);
                digitalWrite(12,LOW);
                // update status on Firebase
                // Wire.begin(10);
                //Serial.println(HourInt);
                // Wire.write(20);
                // Wire.write(true);
                // logSnd(HourInt);
                break;
              }

              MinStr =  String(now.minute(), DEC); // Getting Minutes is save into String
              MinInt = MinStr.toInt();
              Serial.println (MinInt);
              if (MinInt  - tempMin > 0) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("You missed ");
                lcd.setCursor(0, 1);
                lcd.print("your medicine");
                delay(1000);
                // update on FB
                break;
              }
            }
          }
        }
      }
    }

    lcd.clear();

    // now we will check if the medicine is out of stock
    for ( int x = 0 ; x < medSize ; x++) {

      if (med[x].getContainerNum() == 0) {
        outOfStock = scale1.get_units(), 10;
        if (outOfStock < 0.20) {
          med[x].setStockState(false);
          LedReturn=Led(13);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Your medicine ");
          lcd.print(x + 1);
          lcd.setCursor(0, 1);
          lcd.print("is out stock");
          delay(1500);
          lcd.clear();
          //print to lcd and update to the firebase

        }
      } else if (med[x].getContainerNum() == 1) {
        outOfStock = scale2.get_units(), 10;
        if (outOfStock < 0.80) {
          LedReturn=Led(13);
          med[x].setStockState(false);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Your medicine ");
          lcd.print(x + 1);
          lcd.setCursor(0, 1);
          lcd.print("is out stock");
          delay(1500);
          lcd.clear();
          //print to lcd and update to the firebase
        } else {
          med[x].setStockState(true);
        }
      }
    }
  }
}

//------------------------------------------------------------

void receiveEvent() {

  while (0 < Wire.available()) {

    medSize = Wire.read();
    //*******************

    ID = Wire.read(); // Receive and store the user ID from the esp
    if (ID >= 1) {
      uID = ID - 1;
      user[uID].setID(ID);
      Serial.print("User ID: ");
      Serial.println(user[uID].getID());
      //*******************

      strLen = Wire.read();
      for (int i = 0; i < strLen ; i++) {
        c = Wire.read();
        uName += c;
      }
      user[uID].setUname(uName);
      Serial.println(user[uID].getUname());
      uName = "";

      //*******************
      ConNum = Wire.read();
      med[ConNum].setContainerNum(ConNum);

      strLen = Wire.read();
      for (int i = 0; i < strLen ; i++) {
        c = Wire.read(); // Receive and store the Dosage amount value char by char
        Dosage += c;    // And store it into the Dosage varriable
      }
      med[ConNum].setDosage(Dosage);
      Serial.println(med[ConNum].getDosage());
      Dosage = "";
      //*******************

      DosageNum = Wire.read(); // Receive and store the number of doses value from the esp
      Serial.println(DosageNum);
      med[ConNum].setDosesNum(DosageNum);

      for (int i = 0 ; i < DosageNum ; i++) {
        DTime = Wire.read(); // Receive and store the Dosage time value from the esp
        Serial.println(DTime);
        med[ConNum].setTimes(DTime, i);

        DMin = Wire.read(); // Receive and store the Dosage time value from the esp
        Serial.println(DMin);
        med[ConNum].setMinutes(DMin, i);
      }
      //*******************

      InStock = Wire.read(); // Read the In stock value from the esp
      med[ConNum].setStockState(InStock);
      Serial.print("In Stock ? ");
      Serial.println(med[ConNum].getStockState());

      Serial.println();            //  to newline
      GotIt = 1; //Set the flag to 1 to confirm that the data is delivered
      Serial.print("Got it");
      Serial.println(GotIt);
      user[uID].setMed(med[ConNum]);
    }
  }
}
//------------------------------------------------------------

void SettingUp() {

  lcd.setCursor(0, 0);// First digit , First line
  lcd.print("Getting Data");
  lcd.setCursor(0, 1);// First digit , Second line
  lcd.print("Please wait ...");

  while (GotIt != 1) {// the program wil wait untill the flag is set to 1 in order to procede
    Serial.println();
  }

  lcd.clear();// Clear the screen from everything
  // First container
  scale1.set_scale();
  scale1.tare();  //Reset the scale to 0
  scale2.set_scale();
  scale2.tare();  //Reset the scale to

  long zero_factor1 = scale1.read_average(); //remove the need of tare
  long zero_factor2 = scale2.read_average(); //remove the need of tare



  lcd.setCursor(0, 0);// First digit , First line
  lcd.print("SmartMedicalBox");
  lcd.setCursor(0, 1);// First digit , Second line
  lcd.print("****Welcome****");
  delay(2000);

  lcd.clear();// Clear the screen from everything
  lcd.setCursor(0, 0);
  lcd.print("Hello ");
  lcd.setCursor(0, 1);
  lcd.print(user[uID].getUname());
  delay(2000);

  for (int i = 0 ; i < medSize; i++) {
    lcd.clear();// Clear the screen from everything
    lcd.setCursor(0, 0);
    lcd.print("Please add");
    lcd.setCursor(0, 1);
    lcd.print(" a medicine ");
    lcd.print( i + 1 );

    if (med[i].getContainerNum() == 0) {// For the first container
      scale1.set_scale(calibration_factor); //Adjust to this calibration factor
      units1 = scale1.get_units(), 10;

      while (units1 < 0.20) {// Wait untill the user to put his medicine
        units1 = scale1.get_units(), 10;

        delay(1000);
        if (units1 < 0)
        {
          units1 = 0.00;// to avoid getting minus weight values
        }
      }
    } else if (med[i].getContainerNum() == 1) {//for the second container
      scale2.set_scale(calibration_factor2); //Adjust to this calibration factor
      units2 = scale2.get_units(), 10;

      while (units2 < 1.00) {// Wait untill the user to put his medicine
        units2 = scale2.get_units(), 10;

        delay(1000);
        if (units2 < 0)
        {
          units2 = 0.00;// to avoid getting minus weight values
        }
      }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Medicine ");
    lcd.print( i + 1 );
    lcd.print(" added");
    lcd.setCursor(0, 1);

    if (med[i].getContainerNum() == 0) {
      firstWeight = units1;
      lcd.print(firstWeight);
      Serial.print(firstWeight);
      Serial.print(" grams");
      delay(2000);
      lcd.clear();
    }
    else if (med[i].getContainerNum() == 1) {
      firstWeight = units2;
      lcd.print(firstWeight);
      Serial.print(firstWeight);
      Serial.print(" grams");
      delay(2000);
      lcd.clear();
    }
  }
}
//------------------------------------------------------------

int AlarmTone() { //this method will activate the buzzer and play a tone
  tone(buzzer, 1000);//Play a tone for pin 4 1000 Hz
  delay(500);
  tone(buzzer, 1500);//Play a tone for pin 4 1500 Hz
  delay(500);
  tone(buzzer, 2000);//Play a tone for pin 4 2000 Hz
  delay(500);
  noTone(buzzer);//Stop the tone
  return 1;
}
//------------------------------------------------------------
/*
  void takenSnd() {
  Wire.write(20);
  Wire.write(true);
  }
  
*/
int Led(int pin){
   digitalWrite(pin,HIGH);
  delay(500);
  digitalWrite(pin,LOW);
  delay(500);
  return 1;
}
//void testAlarmLED(int expectedLED,int RealLED, int expectedAlarm ,int RealAlarm){
//  
//  Serial.print("Expected LED Output = ");
//  Serial.println(expectedLED);
//  Serial.print("Real LED Output= ");
//  Serial.println(RealLED);
//
//  Serial.print("Expected Alarm Output = ");
//  Serial.println(expectedAlarm);
//  Serial.print("Real Alarm Output= ");
//  Serial.println(RealAlarm);
//  
//  if (RealLED == RealAlarm ){
//    Serial.println("Test LED & Alarm passed");
//  }
//  else{
//    Serial.println("Test LED & Alarm failed");
//  }
//}
