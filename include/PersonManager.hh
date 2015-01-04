#ifndef __PERSONMANAGER_HH
#define __PERSONMANAGER_HH

//
// Person Manager class. 
//
//


#include "Base.hh"
#include <vector>



//Foward Declarations
class Person;
class PersonLogger;



class PersonManager : public Base {
public:
  PersonManager();

  ~PersonManager();

  void BuildList(int NumberOfPeople);
  void BuildCompleteNetwork(int NumberOfPeople);

  void Initialize();

  void PrintConnections();
  void PrintHavesWants();
  void PrintHavesWants(int);
  void PrintMoney();
  void MakeTransactions();

  void SetPersonToLog(int v);
  
  PersonLogger* GetLogger();


private:
  int rInitialTopConectivity;
  int rNumInteractingPeoplePerStep;
  int rNumPeople;
  std::vector <Person> rTheListOfPeople;

  int rPersonToLog;
  PersonLogger * rTheLogger;
};


#endif
