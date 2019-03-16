#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "smb-499-8a7d2.firebaseio.com" // DataBase Host Name
#define FIREBASE_AUTH "XGKBIJ5TbwDq6aCXF85J2pe77VhcpVXVTohI0uzZ" //token to acceses the DB
#define WIFI_SSID "Moodyzz" // name of the router 
#define WIFI_PASSWORD "14161416" // password of the router
int len;
int takenTime=0;
int confirm = -1;
void setup() {
  Serial.begin(9600);
  Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) { //here will print dots untill the connection succes
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());// IP address
  //connect to FireBase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Wire.beginTransmission(8);

  
  int responce = 0;
  int takenTime=0;
  /**while (responce == 0) {
    Wire.requestFrom(8, 1);
    while (Wire.available()) {
      responce = Wire.read();
      Serial.println("responce nested loop");
      delay(500);
    }
    Serial.println(responce);
     delay(500);
    }
    Wire.write(1);**/


  while (true) {
    Serial.println("confirm loop");
    
    SettingUp();
    delay(2000);
    //Wire.requestFrom(8, 4);
   // while (Wire.available()) {
      
      Serial.println("responce nested loop");
      
      Serial.print("confirm: ");
      Serial.println(confirm);
      if (confirm == 0) {
        break;
      }
    //}

  }
}


void loop() {

  boolean flag = Firebase.getBool("/Flag");
  if (flag == 1) {
    SettingUp();

    Firebase.setBool("/Flag", false);
    

    }
 /**   Wire.requestFrom(8, 1);
    while (Wire.available()) {
      takenTime = Wire.read();
      Serial.print("log:");
      Serial.println(takenTime);
     // boolean state = Wire.read();
     Firebase.pushInt("/taken", takenTime);
  }**/

}
void SettingUp() {
  // the adress to send should be 8 in both ESP/Arduino
  //getting information from the data base
  Wire.beginTransmission(8);
  int userID = Firebase.getInt("/User/ID");
  if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }
  Wire.write(userID);
  String uName = Firebase.getString("/User/Name"); // getting the username
  if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  } 
  len = uName.length(); // lenght of the username

  Wire.write(len); // sending the lenght of the username
  for (int i = 0; i < len; i++) {
    Wire.write(uName.charAt(i));// sending character by character
  }

  int containerNum = Firebase.getInt("/Medicine/container");
  if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }
  Wire.write(containerNum);
  String MedName = Firebase.getString("/Medicine/Name");
  if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }
  len = MedName.length();
  Wire.write(len);
  for (int i = 0; i < len; i++) {
    Wire.write(MedName.charAt(i));
  }


  String Dosage = Firebase.getString("/Medicine/Dosage");
  if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }
  len = Dosage.length();

  Wire.write(len);
  for (int i = 0; i < len; i++) {
    Wire.write(Dosage.charAt(i));
  }



  int DosageNum = Firebase.getInt("/Medicine/DosesNum");
  if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }
  Serial.println(DosageNum);
  Wire.write(DosageNum);
  String d = "/Medicine/Times/Dose";
  String s = "/Medicine/TakenStatus/State";

  for (int i = 0; i < DosageNum; i++) {
    String Dosepath = d + i;
    int fDose = Firebase.getInt(Dosepath);
    if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }
    Wire.write(fDose);
    Serial.print(fDose);

  }
  boolean inStock = Firebase.getBool("/Medicine/InStock");
  if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }
  Wire.write(inStock);
 confirm = Wire.endTransmission();
}
