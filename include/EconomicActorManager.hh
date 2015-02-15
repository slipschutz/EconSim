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
class EconomicActor;
class PersonLogger;



class EconomicActorManager : public Base {
public:
  EconomicActorManager();

  ~EconomicActorManager();

  void BuildList(int NumberOfPeople);
  void BuildCompleteNetwork(int NumberOfPeople);

  void Initialize();

  void PrintConnections();
  void PrintHavesWants();
  void PrintHavesWants(int);
  void PrintMoney();

  void DoAStep();

  void KillActor(EconomicActor*);

  //  void SetPersonToLog(int v);
  vector <EconomicActor*> * GetList(){return &rTheListOfActors;}

  void Test(){
    KillActor(rTheListOfActors[4]);
  }

private:
  int rInitialTopConectivity;
  int rNumInteractingPeoplePerStep;
  int rNumPeople;
  std::vector <EconomicActor*> rTheListOfActors;

  int rPersonToLog;

  int rNumberOfDeaths;
};


#endif
