#include <Wire.h>
String s;
void setup() {
 Wire.begin(8);                /* join i2c bus with address 8 */
 Wire.onReceive(receiveEvent); /* register receive event */
// Wire.onRequest(requestEvent); 
 Serial.begin(9600);           /* start serial for debug */
}

void loop() {
 receiveEvent(40);
}

// function that executes whenever data is received from master

String uName ;
int uID =0;
String MedName;
String MedType;
String Dosage;
int DosageNum=0 ;
boolean Tstate;
int t =0 ;

char c ;
int strLen=0;


void receiveEvent(int HowMany) {
 while (0 <Wire.available()) {
  
    strLen = Wire.read();      /* receive byte as a character */  
    for (int i = 0; i<strLen ; i++){
      c= Wire.read();
      uName += c;
    }
    Serial.println("User Name: "+uName);
    
    uID = Wire.read();  
    Serial.print ("User ID: ");
    Serial.println (uID);
    
    strLen = Wire.read();
     for (int i = 0; i<strLen ; i++){
      c= Wire.read();
      MedName += c;
    }
    Serial.println("MedName: "+MedName);
    
    strLen = Wire.read();
     for (int i = 0; i<strLen ; i++){
      c= Wire.read();
      Dosage += c;
  }
  Serial.println("Dosage: "+Dosage);
  
  strLen = Wire.read();
  for(int i = 0 ; i<strLen; i++){
      c= Wire.read();
      MedType += c;
  }
  Serial.println("Med Type"+MedType);



  DosageNum = Wire.read();

  
Serial.println(DosageNum);
    
 
  
   for(int i =0 ; i<DosageNum ; i++){
    t = Wire.read();
   Serial.println(t);
   }
   
 Serial.println();             /* to newline */
}
}



// function that executes whenever data is requested from master
/*void requestEvent() {
  int i  = 9;
 Wire.write(i);  /*send string on request 
}*/
