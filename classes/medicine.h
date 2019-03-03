class medicine {
  private:
  int MedID;// medicine id
  String Type;// type of the medicine. ex:syrup , pill
  String Dosage;// the amount of each dose
  boolean InStock;// to check if the medicine still available or not
  boolean Taken;//to check if the user took his medicine or not

   public:
          void setMedID(int id){
            MedID=id;
          }
          int getMedID(){
            return MedID;
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
          
};

