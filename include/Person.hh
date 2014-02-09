
#ifndef __PERSON_HH
#define __PERSON_HH

#include "Base.hh"
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

  inline map <int,Person*>* GetConnections(){return &rConnections;}
  inline int GetUniqueId(){return rUniqueId;}
private:
  
  long rNumConnections;
  int rUniqueId;
  static int rNext_Id;
  static int GetNextId();

  map <int,Person*> rConnections;
  

};
#endif
