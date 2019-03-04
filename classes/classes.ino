#include "medicine.h"
#include "user.h"
 medicine med[2];
  User user;
void setup() {// main class , will add later 
  Serial.begin(9600);
  user.setID(1);
  user.setUname("Ahmed");
  
  
  med[0].setMedID(1);
  med[0].setMedName("Panadol");
  med[0].setType("Pills");
  med[0].setDosage("1 pill");
  med[0].setContainerNum(1);
  med[0].setStockState(true);
  med[0].setTakenState(false);
  
  med[0].setDosesNum(3);
  for(int i = 0; i<med[0].getDosesNum();i++){
  med[0].setTimes("11",i);
  }
  user.setMed(med[0]);
  
}

void loop() {
  Serial.println(user.getID());
  Serial.println(user.getUname());
  Serial.println("user medicine: "+user.getMed().getMedName());
  Serial.println(med[0].getMedID());
  Serial.println(med[0].getMedName());
  Serial.println(med[0].getType());
  Serial.println(med[0].getDosage());
  Serial.println(med[0].getContainerNum());
  Serial.println(med[0].getStockState());
  Serial.println(med[0].getTakenState());
  Serial.println(med[0].getDosesNum());
  
  for(int i = 0; i<med[0].getDosesNum();i++){
  Serial.println(med[0].getTimes(i));
  }
  
delay(30000);
}
