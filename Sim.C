

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
  EconomicActorManager man;
  man.BuildTestNetwork();
  
  for (int i=0;i<2;i++){
    man.DoAStep();
    MarketManager::Get()->ClearMarket();
    Calendar::DayNumber++;
    man.PrintAllInfo();
    GoodManager::Get()->Dump();
    cout<<"______________________"<<endl;
  }

  ActorLogger::Get()->DumpLog();
  GoodManager::Get()->Dump();
}



int main(int argv, char ** argc){
  // TestGoodStuff();
  // cout<<"\n\n\n";
  // GoodManager::Get()->Dump();

  // return 0;
  //  return 3;

  try{
    EconomicActorManager *  theManager = new EconomicActorManager();
    
    DataLogger::Get();
    
    theManager->BuildCompleteNetwork(Settings::NumberOfPeople);
  
    cout<<"BUILT NETWORK"<<endl;

  
    for (int i=0;i<Settings::NumberOfSteps;i++){
    
      theManager->DoAStep();

      if (i%1==0){
	cout<<"ON "<<i<<endl;
      }
      if (i % 1 == 0){
	// GoodManager::Get()->Dump();
	// MarketManager::Get()->DumpCurrentGoodsForSale();
	// cout<<"______________"<<endl;
	DataLogger::Get()->LogMarketState(MarketManager::Get(), GoodManager::Get());
	DataLogger::Get()->LogPopulation(theManager->GetNumberOfPeople());
      }

      MarketManager::Get()->ClearMarket();
      Calendar::DayNumber++;

    }


 
    

    //theManager.PrintMoney();
    // theManager.PrintHavesWants();
    DataLogger::Get()->LogEndingMoneyDistribution(theManager->GetList());

    delete theManager;
    //    GoodManager::Get()->Dump();
    ActorLogger::Get()->DumpLog();


    if (DataLogger::Get() != NULL){
      delete DataLogger::Get();
    }
  } catch (exception &e){
    cout<<"*******************************************************"<<endl;
    cout<<"Fatal Exception Must exit"<<endl;
    cout<<e.what()<<endl;
    cout<<"*******************************************************"<<endl;
  }
  

   return 0;
}
