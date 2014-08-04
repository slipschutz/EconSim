
#ifndef __PERSON_HH
#define __PERSON_HH

#include "Unique.hh"
#include "ErrorManager.hh"
#include <sstream>

#include <map>
using namespace std;


class Person : public Base {
public:
  Person();
  ~Person();
 
  void Initialize();
  void MakeConnection(Person *); // Makes a connection betwen two people
  void IncrementConnections(){rNumConnections++;}
  bool CheckConnection(Person*);
  void DumpConnections();

  inline long GetNumConnections(){return rNumConnections;}
  inline map <int,Person*>* GetConnections(){return &rConnections;}
  
private:
    
  long rNumConnections;

  map <int,Person*> rConnections;
  double rMoney;

};
#endif
