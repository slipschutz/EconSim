#ifndef __PERSONMANAGER_HH
#define __PERSONMANAGER_HH

//
// Person Manager class. 
//
//


#include "Base.hh"

class PersonManager : public Base {
public:
  
  PersonManager* GetInstance();
  PersonManager* Get();
  PersonManager();

  ~PersonManager();
private:


};


#endif
