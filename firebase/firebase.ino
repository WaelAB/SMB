#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "smb-499-8a7d2.firebaseio.com" // DataBase Host Name
#define FIREBASE_AUTH "XGKBIJ5TbwDq6aCXF85J2pe77VhcpVXVTohI0uzZ" //token to acceses the DB
#define WIFI_SSID "Moodyzz" // name of the router 
#define WIFI_PASSWORD "14161416" // password of the router

int len;// Length for every string

int userID = 0;
String uName ;
int containerNum = 0;
String MedName ;
String Dosage ;
int DosageNum = 0 ;
boolean flag ;


void setup() {
  Serial.begin(9600);
  Wire.begin(D1, D2); // join i2c bus with SDA=D1 and SCL=D2 of NodeMCU
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
  Wire.beginTransmission(8); // the adress to send should be 8 in both ESP/Arduino
  //getting information from the data base

  userID = Firebase.getInt("/User/ID");
  Wire.write(userID);
  //******************

  uName = Firebase.getString("/User/Name"); // getting the username
  len = uName.length(); // lenght of the username
  Wire.write(len); // sending the lenght of the username
  for (int i = 0; i < len; i++) {
    Wire.write(uName.charAt(i));// sending character by character
  }
  //******************

  containerNum = Firebase.getInt("/Medicine/container");
  Wire.write(containerNum);
  //******************

  MedName = Firebase.getString("/Medicine/Name");
  len = MedName.length();
  Wire.write(len);
  for (int i = 0; i < len; i++) {
    Wire.write(MedName.charAt(i));
  }
  //******************

  Dosage = Firebase.getString("/Medicine/Dosage");
  len = Dosage.length();
  Wire.write(len);
  for (int i = 0; i < len; i++) {
    Wire.write(Dosage.charAt(i));
  }
  //******************

  DosageNum = Firebase.getInt("/Medicine/DosesNum");
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
  //******************

  boolean inStock = Firebase.getBool("/Medicine/InStock");
  Wire.write(inStock);
  Wire.endTransmission();


  Serial.print("end");
}



void loop() {

  flag = Firebase.getBool("/Flag");
  if (flag == 1) {
    Wire.beginTransmission(8); // the adress to send should be 8 in both ESP/Arduino
    //getting information from the data base
    userID = Firebase.getInt("/User/ID");
    Wire.write(userID);
    uName = Firebase.getString("/User/Name"); // getting the username
    len = uName.length(); // lenght of the username

    Wire.write(len); // sending the lenght of the username
    for (int i = 0; i < len; i++) {
      Wire.write(uName.charAt(i));// sending character by character
    }

    containerNum = Firebase.getInt("/Medicine/container");
    Wire.write(containerNum);
    MedName = Firebase.getString("/Medicine/Name");
    len = MedName.length();
    Wire.write(len);
    for (int i = 0; i < len; i++) {
      Wire.write(MedName.charAt(i));
    }


    Dosage = Firebase.getString("/Medicine/Dosage");
    len = Dosage.length();

    Wire.write(len);
    for (int i = 0; i < len; i++) {
      Wire.write(Dosage.charAt(i));
    }


    DosageNum = Firebase.getInt("/Medicine/DosesNum");
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
    Serial.print("end");

    Firebase.setBool("/Flag", false);
  }
}
