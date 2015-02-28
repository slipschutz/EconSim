


#include "Death.hh"
#include "Exceptions.hh"
#include "Calendar.hh"
#include <sstream>
Death::Death(){

}

Death * Death::theManager=NULL;

Death * Death::Get(){

  if (theManager == NULL){
    theManager = new Death();
  }
  return theManager;

}

void Death::AddDead(int id ,int day){
  auto it = TheDead.find(id);
  
  if ( it != TheDead.end()){//Dead already Dead
    stringstream s;s<<"Dead Man "<<id<<" already Dead"<<endl;
    s<<"Died on "<<it->second<<" it is "<<day<<endl;
    MessageException(s.str());
  }else{
    TheDead[id]=day;
  }
}


void Death::FindDead(int id){
  auto it = TheDead.find(id);
  
  if ( it != TheDead.end() ){
    cout<<"That id "<<id<<" is cold in the ground"<<endl;
    cout<<"Died on "<<it->second<<endl;
    cout<<"Today is "<<Calendar::DayNumber<<endl;
  } else {
    cout<<"IT still draws breath"<<endl;
  }
    

}
