

//Generral Headers
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Person.hh"

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


#include "PythonWrapper/Run.hh"

using namespace  std;


#include "PersonTraitPact.hh"

int main(int argv, char ** argc){

  for (int i=0;i<10;i++){
    cout<<RandomManager::GetRand(10)<<endl;
  }
  return 0;
  Run();
  return 0;
}
