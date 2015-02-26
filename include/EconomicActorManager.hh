#ifndef __PERSONMANAGER_HH
#define __PERSONMANAGER_HH

//
// Person Manager class. 
//
//


#include "Base.hh"
#include <vector>
#include <map>
#include <unordered_map>
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
  void MakeActor(EconomicActor*);

  //  void SetPersonToLog(int v);
  unordered_map <int,EconomicActor*> * GetList(){return &rTheListOfActors;}

  void Test(){
    //    KillActor(rTheListOfActors[4]);
  }

private:
  int rInitialTopConectivity;
  int rNumInteractingPeoplePerStep;
  int rNumPeople;
  std::unordered_map <int,EconomicActor*> rTheListOfActors;

  vector <int> rTheIds;///<Vector that is used to determine the order that the Actors act in

  int rPersonToLog;

  int rNumberOfDeaths;
  int rNumberOfBirths;
};


#endif
