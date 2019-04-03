class User {
  private:
         int ID;// id of the user
         String uname;// name of the user
         medicine Med;
  public:// sets and gets methods
         void setID(int id){
          ID=id;
         }
         int getID(){
          return ID;
         }
         void setUname(String name){
          uname=name;
         }
         String getUname(){
          return uname;
         }
         void setMed(medicine med){
          Med=med;
         }
         medicine getMed(){
          return Med;
         }
         
};
