

//General Headers
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


#include "Network.hh"

#include "PythonWrapper/Run.hh"

#include <gtest/gtest.h>

using namespace  std;


#include "PersonTraitPact.hh"



int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// int main(int argv, char ** argc){

//   UnitTests();

//   //  Run();
//   return 0;  
// } 
 
