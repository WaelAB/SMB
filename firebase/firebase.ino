#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "smb-499-8a7d2.firebaseio.com" // DataBase Host Name
#define FIREBASE_AUTH "XGKBIJ5TbwDq6aCXF85J2pe77VhcpVXVTohI0uzZ" //token to acceses the DB
#define WIFI_SSID "Moodyzz" // name of the router 
#define WIFI_PASSWORD "14161416" // password of the router
int len;
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
  Wire.beginTransmission(8);

  int confirm = 0;
  int responce = 0;
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


  while (confirm == 0) {
    Serial.println("confirm loop");
    
    SettingUp();
    delay(1000);
    Wire.requestFrom(8, 1);
    while (Wire.available()) {
      
      Serial.println("responce nested loop");
      confirm = Wire.read();
      Serial.print(confirm);
      if (confirm == 1) {
        break;
      }
    }

  }
}


void loop() {

  boolean flag = Firebase.getBool("/Flag");
  if (flag == 1) {
    SettingUp();

    Firebase.setBool("/Flag", false);
    while (Wire.available()) {
      int takenTime = Wire.read();
      boolean state = Wire.read();
      String name = Firebase.pushInt("/logs", takenTime);

    }
  }

}
void SettingUp() {
  // the adress to send should be 8 in both ESP/Arduino
  //getting information from the data base
  Wire.beginTransmission(8);
  int userID = Firebase.getInt("/User/ID");
  Wire.write(userID);
  String uName = Firebase.getString("/User/Name"); // getting the username
  len = uName.length(); // lenght of the username

  Wire.write(len); // sending the lenght of the username
  for (int i = 0; i < len; i++) {
    Wire.write(uName.charAt(i));// sending character by character
  }

  int containerNum = Firebase.getInt("/Medicine/container");
  Wire.write(containerNum);
  String MedName = Firebase.getString("/Medicine/Name");
  len = MedName.length();
  Wire.write(len);
  for (int i = 0; i < len; i++) {
    Wire.write(MedName.charAt(i));
  }


  String Dosage = Firebase.getString("/Medicine/Dosage");
  len = Dosage.length();

  Wire.write(len);
  for (int i = 0; i < len; i++) {
    Wire.write(Dosage.charAt(i));
  }



  int DosageNum = Firebase.getInt("/Medicine/DosesNum");
  Serial.println(DosageNum);
  Wire.write(DosageNum);
  String d = "/Medicine/Times/Dose";
  String s = "/Medicine/TakenStatus/State";

  for (int i = 0; i < DosageNum; i++) {
    String Dosepath = d + i;
    int fDose = Firebase.getInt(Dosepath);
    Wire.write(fDose);
    Serial.print(fDose);

  }
  boolean inStock = Firebase.getBool("/Medicine/InStock");
  Wire.write(inStock);
  Wire.endTransmission();
}
