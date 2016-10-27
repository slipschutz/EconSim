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

#include "Settings.hh"
using namespace std;


//Foward Declarations
class EconomicActor;
class Manufacturer;
class Person;
class Company;


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

  void MarkForDeath(EconomicActor*);
  void MakeActor(EconomicActor*);

  //  void SetPersonToLog(int v);
  unordered_map <int,EconomicActor*> * GetList(){return &rTheListOfActors;}

  void Test(){
    cout<<"Size of list "<<rTheListOfActors.size()<<endl;
  }
  
  Person * FindPerson(int Id);
  Company * FindCompany(int Id);
  
  void BuildTestNetwork();

  void PrintAllInfo();
  
  int GetNumberOfPeople(){return Settings::NumberOfPeople-rNumberOfPeopleDeaths;}

private:
  int rInitialTopConectivity;
  int rNumInteractingPeoplePerStep;
  int rNumberOfPeople;
  
  int rNumCurrentCompanies;
  std::unordered_map <int,EconomicActor*> rTheListOfActors;

  vector <int> rTheIds;///<Vector that is used to determine the order that the Actors act in
  vector <int> rToBeKilled;

  int rPersonToLog;

  int rNumberOfPeopleDeaths;
  int rNumberOfPeopleBirths;
  int rNumberOfCompanyBirths;
  int rNumberOfCompanyDeaths;
  int rNumberOfManufacturers;
  int rNumberOfBirths;
};


#endif
