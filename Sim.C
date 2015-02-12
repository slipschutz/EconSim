

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

#include "Settings.hh"
#include "DataLogger.hh"

#include "GoodManager.hh"
#include "MarketManager.hh"
using namespace  std;

int main(int argv, char ** argc){

  MarketManager::Get()->PlaceSellOrder(3,1,4,4.1);
  MarketManager::Get()->PlaceSellOrder(3,12,5,42.4);
  MarketManager::Get()->PlaceSellOrder(44,13,6,45.4);
  MarketManager::Get()->PlaceSellOrder(44,14,6,14.4);
  
  MarketManager::Get()->Dump();
  return 0;
  
  EconomicActorManager theManager;
  
  DataLogger::Get();

  theManager.BuildCompleteNetwork(Settings::NumberOfPeople);
  //  theManager.SetPersonToLog(3);

  //  theManager.PrintConnections();

  cout<<"BUILT NETWORK"<<endl;

  // theManager.PrintHavesWants();

  for (int i=0;i<Settings::NumberOfSteps;i++){
    //theManager.PrintHavesWants();
    //cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;
    theManager.MakeTransactions();
    //    theManager.PrintHavesWants();

    if (i%100==0){
      cout<<"ON "<<i<<endl;
    }
  }

  //theManager.PrintMoney();
  // theManager.PrintHavesWants();
  //  DataLogger::Get()->LogEndingMoneyDistribution(theManager.GetList());

  if (DataLogger::Get() != NULL){
    delete DataLogger::Get();
  }
  return 0;
}
