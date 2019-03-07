#include "medicine.h"
#include "user.h"
#include <Wire.h>

medicine med[2];
User user[2];
 
void setup() {
 Wire.begin(8);                /* join i2c bus with address 8 */
 Wire.onReceive(receiveEvent); /* register receive event */
// Wire.onRequest(requestEvent); 
 Serial.begin(9600);           /* start serial for debug */
 
}

void loop() {
 receiveEvent();
}

// function that executes whenever data is received from master

void receiveEvent() {
  
String uName ; // User Name
int uID; // User ID
String MedName; // Medicine Name
String Dosage; // Dosage amount
int DosageNum=0 ; // The number of doses
boolean InStock; 
int DTime =0  ; // Dosage Time
int ConNum =0 ; // Container Number
char c ; // Temporary varible to store chars
int strLen=0; // Temporary varible to store string length

 while (0 <Wire.available()) {  
    
    uID = Wire.read(); // Receive and store the user ID from the esp
    
    user[uID].setID(uID);
    Serial.print("User ID: ");
    Serial.println(user[uID].getID());
    //******************* 
    
    strLen = Wire.read();      
    for (int i = 0; i<strLen ; i++){
      c= Wire.read();
      uName += c;
    }
    user[uID].setUname(uName); 
    Serial.println(user[uID].getUname());
    uName ="";
    //*******************
    
    ConNum = Wire.read();
    med[ConNum].setContainerNum(ConNum);
    Serial.println(med[ConNum].getContainerNum());
    //*******************
      
    strLen = Wire.read();
     for (int i = 0; i<strLen ; i++){
     c= Wire.read(); // Receive and store the Medicine name value char by char
     MedName += c;   // And store it into the Dosage varriable 
    }
 
    med[ConNum].setMedName(MedName); 
    Serial.println("MedName: "+ med[ConNum].getMedName());
    MedName= "";
    //*******************
    
    strLen = Wire.read();
     for (int i = 0; i<strLen ; i++){
      c= Wire.read(); // Receive and store the Dosage amount value char by char
      Dosage += c;    // And store it into the Dosage varriable
  }
    med[ConNum].setDosage(Dosage);
    Serial.println(med[ConNum].getDosage()); 
    Dosage = "";
    //*******************

    DosageNum = Wire.read(); // Receive and store the number of doses value from the esp
    Serial.println(DosageNum);
    for(int i =0 ; i<DosageNum ; i++){
    DTime = Wire.read(); // Receive and store the Dosage time value from the esp
    med[ConNum].setTimes(DTime,i);
    Serial.print("Time: ");
    Serial.println(med[ConNum].getTimes(i));
    }
    //*******************
  
    InStock = Wire.read(); // Read the In stock value from the esp
    med[ConNum].setStockState(InStock);
    Serial.print("In Stock ? ");
    Serial.println(med[ConNum].getStockState());
  
    Serial.println();            //  to newline 
    user[uID].setMed(med[ConNum]);
}
}


// Will use this later
// function that executes whenever data is requested from master
/*void requestEvent() {
 int i  = 9;
 Wire.write(i);  /*send string on request 
}*/
