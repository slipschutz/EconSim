

//Generral Headers
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Person.hh"
#include "MatrixMaker.hh"
#include "Temp.hh"
#include "PersonManager.hh"
#include "RandomManager.hh"

#include "Settings.hh"
#include "DataLogger.hh"

using namespace  std;

int main(int argv, char ** argc){
    
  PersonManager theManager;
  
  DataLogger::Get();

  theManager.BuildCompleteNetwork(Settings::NumberOfPeople);
  theManager.SetPersonToLog(3);

  //theManager.PrintConnections();

  cout<<"BUILT NETWORK"<<endl;

  theManager.PrintHavesWants();

  for (int i=0;i<Settings::NumberOfSteps;i++){
    //theManager.PrintHavesWants();
    //cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;
    theManager.MakeTransactions();
    //    theManager.PrintHavesWants();

    if (i%100==0){
      cout<<"ON "<<i<<endl;
    }
  }

  theManager.PrintMoney();
  // theManager.PrintHavesWants();
  
  if (DataLogger::Get() != NULL){
    delete DataLogger::Get();
  }
  return 0;
}
