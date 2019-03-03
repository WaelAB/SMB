class medicine {
  private:
  int MedID;// medicine id
  String MedName; // name of the medicine
  String Type;// type of the medicine. ex:syrup , pill
  String Dosage;// the amount of each dose
  int DosesNum; // number of doses per day
  boolean InStock;// to check if the medicine still available or not
  boolean Taken;//to check if the user took his medicine or not
  String Times[];// to store the times for single medicine

   public:
          void setMedID(int id){
            MedID=id;
          }
          int getMedID(){
            return MedID;
          }
          void setMedName(String name){
            MedName=name;
          }
          String getMedName(){
            return MedName;
          }
          void setType(String type){
            Type=type;
          }
          String getType(){
            return Type;
          }
          void setDosage(String dose){
            Dosage=dose;
          }
          String getDosage(){
            return Dosage;
          }
          void setDosesNum(int num){
            DosesNum=num;
            Times[DosesNum];
          }
          int getDosesNum(){
            return DosesNum;
          }
          void setStockState(boolean state){
            InStock=state;
          }
          boolean getStockState(){
            return InStock;
          }
          void setTakenState(boolean take){
            Taken=take;
          }
          boolean getTakenState(){
            return Taken;
          }
          void setTimes(String times, int i){
            Times[i]=times;
          }
          String getTimes(int i){
            return Times[i];
          }
          
          
};

