class User {
  private:
         int ID;// id of the user
         String uname;// name of the user
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
};

