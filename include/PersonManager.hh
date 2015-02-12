#ifndef __PERSONMANAGER_HH
#define __PERSONMANAGER_HH

//
// Person Manager class. 
//
//


#include "Base.hh"
#include <vector>
using namespace std;


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

  //  void SetPersonToLog(int v);
  vector <Person> * GetList(){return &rTheListOfPeople;}



private:
  int rInitialTopConectivity;
  int rNumInteractingPeoplePerStep;
  int rNumPeople;
  std::vector <Person> rTheListOfPeople;

  int rPersonToLog;

};


#endif
