#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "smb-499-8a7d2.firebaseio.com" // DataBase Host Name
#define FIREBASE_AUTH "XGKBIJ5TbwDq6aCXF85J2pe77VhcpVXVTohI0uzZ" //token to acceses the DB
#define WIFI_SSID "Connect_4G_Router" // name of the router 
#define WIFI_PASSWORD "Aa2468Aa" // password of the router

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
}
int len;


void loop() {
  Wire.beginTransmission(8); // the adress to send should be 8 in both ESP/Arduino
  //getting information from the data base

  
  String uName=Firebase.getString("/User/Name"); // username
  len=uName.length();
  
  Wire.write(len);
  for (int i=0;i<len;i++){
    Wire.write(uName.charAt(i));
  }
 


  
  //Wire.write(uName);  /* sends hello string */
  int ID=Firebase.getInt("/User/ID"); // ID of the user
  Wire.write(ID);
  String MedName=Firebase.getString("/Medicine/Name"); 
  len=MedName.length();
  Wire.write(len);
  for (int i=0;i<len;i++){
    Wire.write(MedName.charAt(i));
  }
  
  
  String Dosage=Firebase.getString("/Medicine/Dosage");
  len=Dosage.length();
  
  Wire.write(len);
  for (int i=0;i<len;i++){
    Wire.write(Dosage.charAt(i));
  }
  
  String Type=Firebase.getString("/Medicine/Type");
  len=Type.length();
  Wire.write(len);
   for (int i=0;i<len;i++){
    Wire.write(Type.charAt(i));
  }
  int DosageNum=Firebase.getInt("/Medicine/DosesNum");
  Wire.write(DosageNum);
  String d="/Medicine/Times/Dose";
  String s="/Medicine/TakenStatus/State";
  for (int i=0;i<DosageNum;i++){
    String Dosepath=d+i;
    Serial.println(Dosepath);
    String fDose=Firebase.getString(Dosepath);
    Serial.println(fDose);
    len=fDose.length();
    Serial.println(len);
    Wire.write(len);
    for(int j=0;j<len;j++){
      Wire.write(fDose.charAt(j));
    }  
  }
  for (int i=0;i<DosageNum;i++){
    String Statuspath=s+i;
    Serial.println(Statuspath);
    boolean fStatus=Firebase.getBool(Statuspath);
    Serial.println(fStatus);
    Wire.write(fStatus);
  }
  
  
  /**boolean fStatus=Firebase.getBool("/Medicine/TakenStatus/First");
  String sDose=Firebase.getString("/Medicine/Times/Second");
  boolean sStatus=Firebase.getBool("/Medicine/TakenStatus/Second");
  String tDose=Firebase.getString("/Medicine/Times/Third");
  boolean tStatus=Firebase.getBool("/Medicine/TakenStatus/Third");
  int cont=Firebase.getInt("/Medicine/Container");
  boolean inStock=Firebase.getBool("/Medicine/InStock");
  Serial.println(uName);
  Serial.println(cont);
  Serial.println(inStock);
  Serial.println(tDose);
  Serial.println(Type);
  **/
  Serial.print("end");
  Wire.endTransmission(); 
  delay(5000);

}
