class medicine {
  private:
    String MedName; // name of the medicine
    String Dosage;// the amount of each dose
    int DosesNum; // number of doses per day
    int ContainerNum; // number of the container
    boolean InStock;// to check if the medicine still available or not
    int Times[3];// to store the times for single medicine
    int Minutes[3]; // to store the Minutes for single medicine


  public:

    void setMedName(String name) {
      MedName = name;
    }
    String getMedName() {
      return MedName;
    }

    void setDosage(String dose) {
      Dosage = dose;
    }
    String getDosage() {
      return Dosage;
    }

    void setDosesNum(int num) {
      DosesNum = num;
      // Times[DosesNum];
      // Minutes[DosesNum];
    }
    int getDosesNum() {
      return DosesNum;
    }

    void setContainerNum(int num) {
      ContainerNum = num;
    }
    int getContainerNum() {
      return ContainerNum;
    }

    void setStockState(boolean state) {
      InStock = state;
    }
    boolean getStockState() {
      return InStock;
    }

    void setTimes(int times, int i) {
      Times[i] = times;
    }
    int getTimes(int i) {
      return Times[i];
    }

    void setMinutes(int Mins, int j) {
      Minutes[j] = Mins;
    }
    int getMinutes(int j) {
      return Minutes[j];
    }
//    void setTimesSize(int ss){
//      Times[ss];
//    }
//    void setMinSize(int sm){
//      Minutes[sm];
//    }


};
