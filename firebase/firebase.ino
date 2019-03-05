
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "smb-499-8a7d2.firebaseio.com" // DataBase Host Name
#define FIREBASE_AUTH "XGKBIJ5TbwDq6aCXF85J2pe77VhcpVXVTohI0uzZ" //token to acceses the DB
#define WIFI_SSID "Connect_4G_Router" // name of the router 
#define WIFI_PASSWORD "Aa2468Aa" // password of the router

void setup() {
  Serial.begin(9600);

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



void loop() {
  //getting information from the data base
  String uName=Firebase.getString("/User/Name"); // username
  int ID=Firebase.getInt("/User/ID"); // ID of the user
  String MedName=Firebase.getString("/Medicine/Name"); 
  String Dosage=Firebase.getString("/Medicine/Dosage");
  String Type=Firebase.getString("/Medicine/Type");
  int DosageNum=Firebase.getInt("/Medicine/DosesNum");
  String fDose=Firebase.getString("/Medicine/Times/First");
  boolean fStatus=Firebase.getBool("/Medicine/TakenStatus/First");
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
  delay(20000);

}
