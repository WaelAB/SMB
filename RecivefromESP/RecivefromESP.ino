#include <Wire.h>
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
    Serial.println(uID);
    //******************* 
    
    strLen = Wire.read();      
    for (int i = 0; i<strLen ; i++){
      c= Wire.read();
      uName += c;
    }
    Serial.println("User Name: "+uName);
    uName ="";
    //*******************
    
    ConNum = Wire.read();
    Serial.println(ConNum);
    //*******************
      
    strLen = Wire.read();
     for (int i = 0; i<strLen ; i++){
     c= Wire.read(); // Receive and store the Medicine name value char by char
     MedName += c;   // And store it into the Dosage varriable
      
    }
    Serial.println("MedName: "+MedName);
    MedName= "";
    //*******************
    
    strLen = Wire.read();
     for (int i = 0; i<strLen ; i++){
      c= Wire.read(); // Receive and store the Dosage amount value char by char
      Dosage += c;    // And store it into the Dosage varriable
  }
    Serial.println("Dosage: "+Dosage); 
    Dosage = "";
    //*******************

  DosageNum = Wire.read(); // Receive and store the number of doses value from the esp
  Serial.println(DosageNum);
  for(int i =0 ; i<DosageNum ; i++){
  DTime = Wire.read(); // Receive and store the Dosage time value from the esp
  Serial.println(DTime);
   }
  //*******************
  
  InStock = Wire.read(); // Read the In stock value from the esp
  Serial.print("In Stock ? ");
  Serial.println(InStock);
  
  Serial.println();            //  to newline 
}
}


// Will use this later
// function that executes whenever data is requested from master
/*void requestEvent() {
 int i  = 9;
 Wire.write(i);  /*send string on request 
}*/
