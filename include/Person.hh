
#ifndef __PERSON_HH
#define __PERSON_HH

#include "Unique.hh"
#include "Good.hh"
#include "ErrorManager.hh"
#include <sstream>

#include <map>
#include <vector>
using namespace std;

class PersonLogger;

class Person : public Base {
public:
  Person();
  ~Person();
 
  void Initialize();
  void MakeConnection(Person *); // Makes a connection betwen two people
  void IncrementConnections(){rNumConnections++;}
  bool CheckConnection(Person*);
  void DumpConnections();

  void MakeTransactions(PersonLogger*);

  void EndOfStep();
  
  inline long GetNumConnections(){return rNumConnections;}
  inline map <int,Person*>* GetConnections(){return &rConnections;}
  
  map<int,Good> GetWants(){return rWants;}
  map<int,Good> GetHaves(){return rHaves;}
  
  void AddAGood(int GoodNumber);
  void RemoveAGood(int GoodNumber);

  void DumpHavesWants();
  
  double GetWorth(Good);
  
  double GetMoney(){return rMoney;}
  void SubtractMoney(double v){rMoney=rMoney-v;}
  void AddMoney(double v){rMoney=rMoney+v;}

private:

  bool CheckTransactionMatch(int,Person*);

  long rNumConnections;

  map <int,Person*> rConnections;
  int rMoney;
  
  map <int,Good> rWants;
  map <int,Good> rHaves;
  
 

};
#endif
