



#include "TraitPact.hh"


void TraitPact::BoundAboveZero(int &val){
  if (val <0){
    val=0;
  }
}

void TraitPact::BoundAboveZero(float & val){
  if (val <0){
    val=0;
  }
}


void TraitPact::BoundZeroToOne(float &val){
  if (val <0){
    val=0;
  }else if (val>1){
    val=1;
  }
}
TraitPact::TraitPact(){
  double x=5;
  BoundAboveZero(x);
  
}
TraitPact::~TraitPact(){

}
