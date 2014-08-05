
#ifndef __PERSON_HH
#define __PERSON_HH

#include "Unique.hh"
#include "ErrorManager.hh"
#include <sstream>

#include <map>
#include <vector>
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

  void MakeTransactions();

  inline long GetNumConnections(){return rNumConnections;}
  inline map <int,Person*>* GetConnections(){return &rConnections;}
  
  void AddAGood(int GoodNumber);
  void RemoveAGood(int GoodNumber);

  void DumpHavesWants();
private:

  bool CheckTransactionMatch(int,Person*);

  long rNumConnections;

  map <int,Person*> rConnections;
  int rMoney;
  
  map <int,bool> rWants;
  map <int,bool> rHaves;
  

};
#endif
