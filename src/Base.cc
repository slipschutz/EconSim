
#include "Base.hh"

int Base::rNextBase_Id=0;
int Base::rNumDeletes=0;

Base::~Base(){ //Deconstructor 
  //std::cout<<"this is Base Deconstructor"<<std::endl;
  Base::rNumDeletes++;
}

Base::Base(){ //Defualt constructor 
  //std::cout<<"Base Constructor"<<std::endl;
  SetBaseId(Base::rGetNextBaseId());
}

Base::Base(const Base& other) {
  SetBaseId(Base::rGetNextBaseId());
}

int Base::rGetNextBaseId(){
  int d=Base::rNextBase_Id++;
  return d;
}
