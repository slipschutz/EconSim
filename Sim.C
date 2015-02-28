

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
#include "Exceptions.hh"
using namespace  std;

void TestGoodStuff(){
  // Person aPerson;
  
  // Manufacturer aManu(1000);
  // Manufacturer anotherManu(1000);
  
  // anotherManu.Initialize();
  // aPerson.Initialize();
  // aManu.Initialize();

  // aPerson.MakeConnection(&aManu);
  // aPerson.MakeConnection(&anotherManu);

  // aPerson.BeginningOfStep();
  // aManu.BeginningOfStep();
  // anotherManu.BeginningOfStep();

  // aPerson.DumpSupplies();
  // aPerson.DumpDemands();
  
  // aManu.DumpSupplies();
  // aManu.DumpDemands();

  // anotherManu.DumpSupplies();
  // anotherManu.DumpDemands();

  // GoodManager::Get()->Dump();

  // MarketManager::Get()->Dump();
  // aPerson.DoStep();
  // MarketManager::Get()->Dump();
  // //  aPerson.DoStep();
  // cout<<"+++++++++++++++++++++++++++"<<endl;

  // GoodManager::Get()->Dump();
  // MarketManager::Get()->Dump();
  // aPerson.DumpSupplies();
  // aPerson.DumpDemands();

  // aManu.DumpSupplies();
  // aManu.DumpDemands();


  // anotherManu.DumpSupplies();
  // anotherManu.DumpDemands();


}



int main(int argv, char ** argc){
  // TestGoodStuff();


  //  return 3;

  try{
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
  } catch (exception &e){
    cout<<"Fatal Exception Must exit"<<endl;
    cout<<e.what()<<endl;
  }
  

   return 0;
}
