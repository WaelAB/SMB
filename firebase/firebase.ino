#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "smb-499-8a7d2.firebaseio.com" // DataBase Host Name
#define FIREBASE_AUTH "XGKBIJ5TbwDq6aCXF85J2pe77VhcpVXVTohI0uzZ" //token to acceses the DB
#define WIFI_SSID "Connect_4G_Router" // name of the router 
#define WIFI_PASSWORD "Aa2468Aa" // password of the router

int len; // store string lenght's to send it to arduino
int takenTime = 0; // intiate the time
int confirm = -1;// confirmation flag intiated with -1
boolean flag ;

void setup() {
  Serial.begin(9600);
  Wire.begin(D1, D2); // join i2c bus with SDA=D1 and SCL=D2 of NodeMCU

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // connect to wifi using SSID and password
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) { // wait untill the wifi connect
    Serial.print(".");//here will print dots untill the connection succes
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());// IP address

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //connect to FireBase using the Host and authintication key for firebase website

  while (true) {// stuck in the loop until it recives confirmation that the data is delivered
    Serial.println("confirm loop");

    SettingUp();
    delay(2000);

    Serial.println("responce nested loop");
    Serial.print("confirm: ");
    Serial.println(confirm);
    if (confirm == 0) {// 0 is the confirmation number
      break;
    }
  }
}


void loop() {

  flag = Firebase.getBool("/Flag"); // this flag will equal 1 when there is a new information in the firebase
  if (flag == 1) {
    SettingUp();

    Firebase.setBool("/Flag", false);// set the flag into false after sending the data


  }
  /**Wire.beginTransmission(10);
  Wire.requestFrom(10, 4);
    while (Wire.available()) {
     takenTime = Wire.read();  // in progress ,recive data and upload it to the firebase
     Serial.print("log:");
     Serial.println(takenTime);
    // boolean state = Wire.read();
    Firebase.pushInt("/taken", takenTime);
    Wire.endTransmission();
}**/
}

void SettingUp() {
  // the adress to send should be 8 in both ESP/Arduino
  //getting information from the data base

  Wire.beginTransmission(8); // start the transmission on port 8
  int userID = Firebase.getInt("/User/ID");// get the user ID form the firebase and store into User ID
  if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }
  Wire.write(userID);// Send the user ID to the Arduino

  String uName = Firebase.getString("/User/Name"); // getting the username from the firebase
  if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }
  len = uName.length(); //store lenght of the username

  Wire.write(len); // sending the lenght of the username to the arduino
  for (int i = 0; i < len; i++) {
    Wire.write(uName.charAt(i));// sending character by character
  }

  int containerNum = Firebase.getInt("/Medicine/container");// get the container number from the firebase
  if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }
  Wire.write(containerNum);// send the container number to the arduino

//  String MedName = Firebase.getString("/Medicine/Name");// get the medicine name from the firebase
//  if (Firebase.failed()) { // check for error
//    Serial.print("setting /number failed:");
//    Serial.println(Firebase.error());
//  }
//  len = MedName.length();// store the medicine name lenngth 
//  Wire.write(len);// send the medicine name length to the arduino
//  for (int i = 0; i < len; i++) {
//    Wire.write(MedName.charAt(i));// sending character by character
//  }

  String Dosage = Firebase.getString("/Medicine/Dosage");// get the dosage amount from the firebase
  if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }
  len = Dosage.length();// store the dosage length 
  Wire.write(len);// send the length to the arduino
  for (int i = 0; i < len; i++) {
    Wire.write(Dosage.charAt(i));// sending character by character
  }
  
  int DosageNum = Firebase.getInt("/Medicine/DosesNum");// get the dosage number from the firebase
  if (Firebase.failed()) { // check for error
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }
  Serial.println(DosageNum);
  Wire.write(DosageNum);// send the dosage number to the arduino
  String d = "/Medicine/Times/Dose"; // path for the doses
  

  for (int i = 0; i < DosageNum; i++) { // to loop over the number of dosage
    String Dosepath = d + i; // modify the loop in each iterate to get all the gosages 
    String fDose = Firebase.getString(Dosepath);
    String temp;
    int fDoseInt=0;
    if (Firebase.failed()) { // check for error
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());
    }
    for (int j=0;j<fDose.length();j++){
      if (fDose[j] != ':'){
        temp += fDose.charAt(j);
      }else{
        fDoseInt = temp.toInt();
        
        Wire.write(fDoseInt);
        temp="";
      }
    }
    fDoseInt = temp.toInt();
    
        Wire.write(fDoseInt);
        temp="";
    
 
Serial.print(fDose);
fDose="";

  }
  boolean inStock = Firebase.getBool("/Medicine/InStock");
  if (Firebase.failed()) { // check for error
    Serial.print("Getting /Data failed:");
    Serial.println(Firebase.error());
  }
  
  Wire.write(inStock);
  
 



  confirm = Wire.endTransmission();
}
