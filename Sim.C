

//Generral Headers
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Person.hh"
#include "MatrixMaker.hh"
#include "Temp.hh"
#include "EconomicActorManager.hh"
#include "RandomManager.hh"

#include "Manufacturer.hh"

#include "DeadActor.hh"

#include "Settings.hh"
#include "DataLogger.hh"
#include "Calendar.hh"

#include "GoodManager.hh"
#include "MarketManager.hh"

#include "ActorLogger.hh"
using namespace  std;

void TestGoodStuff(){
  Person aPerson;
  
  Manufacturer aManu;
  Manufacturer anotherManu;
  
  anotherManu.Initialize();
  aPerson.Initialize();
  aManu.Initialize();

  aPerson.MakeConnection(&aManu);
  aPerson.MakeConnection(&anotherManu);

  aPerson.BeginningOfStep();
  aManu.BeginningOfStep();
  anotherManu.BeginningOfStep();

  aPerson.DumpSupplies();
  aPerson.DumpDemands();
  
  aManu.DumpSupplies();
  aManu.DumpDemands();

  anotherManu.DumpSupplies();
  anotherManu.DumpDemands();

  GoodManager::Get()->Dump();

  MarketManager::Get()->Dump();
  aPerson.DoStep();
  MarketManager::Get()->Dump();
  //  aPerson.DoStep();
  cout<<"+++++++++++++++++++++++++++"<<endl;

  GoodManager::Get()->Dump();
  MarketManager::Get()->Dump();
  aPerson.DumpSupplies();
  aPerson.DumpDemands();

  aManu.DumpSupplies();
  aManu.DumpDemands();


  anotherManu.DumpSupplies();
  anotherManu.DumpDemands();


}
int main(int argv, char ** argc){
  // TestGoodStuff();

  // vector<EconomicActor*> v;
  // for (int i=0;i<100;i++){
  //   v.push_back(new DeadActor());
  // }
  // cout<<"Done"<<endl;
  // for (int i=0;i<v.size();i++){
  //   delete v[i];
  // }

  // return 3;

  EconomicActorManager *  theManager = new EconomicActorManager();
  
  DataLogger::Get();

  theManager->BuildCompleteNetwork(Settings::NumberOfPeople);

  cout<<"BUILT NETWORK"<<endl;

  
  for (int i=0;i<Settings::NumberOfSteps;i++){
    
    theManager->DoAStep();
    if (i%100==0){
      cout<<"ON "<<i<<endl;
    }
    MarketManager::Get()->ClearMarket();
    Calendar::DayNumber++;
  }


  GoodManager::Get()->Dump();

  //theManager.PrintMoney();
  // theManager.PrintHavesWants();
  DataLogger::Get()->LogEndingMoneyDistribution(theManager->GetList());

  delete theManager;
  
  ActorLogger::Get()->DumpLog();

  if (DataLogger::Get() != NULL){
    delete DataLogger::Get();
  }
  return 0;
}
