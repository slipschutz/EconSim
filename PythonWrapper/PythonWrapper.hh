#include <iomanip>


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

#include "TransactionRecord.hh"

#include "Network.hh"


#include "Run.hh"
using namespace std;

#include "pybind11.h"



///Allows for automatic conversion between STL containers and python types
///However requires a copy everytime types are converted
#include "stl.h" 
namespace py = pybind11;




// vector<double> GetVolumeData(){

//   vector <TransactionRecord>* theGoodPrices=  DataLogger::Get()->GetThePrices();
//   vector  <double> price;
//   cout<<"The size is "<<theGoodPrices->size()<<endl;
//   for ( auto i : *theGoodPrices){
//     price.push_back(i.Supply);
//   }

//   return price;
// }







#include <assert.h>
#include <string>









PYBIND11_PLUGIN(PyEconSim) {
  py::module m("PyEconSim", "pybind11 example plugin");

  m.def("Run", &Run, "Run Simulation");
  m.def("DoEndOfDay", &DoEndOfDay, "Clean Up");
  m.def("UnitTests", &UnitTests, "Run Tests");
  
  
  py::class_<EconomicActorManager>(m, "EconomicActorManager")
    .def(py::init<>())
    .def("DoAStep", &EconomicActorManager::DoAStep)
    .def("BuildCompleteNetwork", &EconomicActorManager::BuildCompleteNetwork)
    .def("GetPriceData", &EconomicActorManager::GetPriceData, "GetPriceData")
    .def("GetSupplies", &EconomicActorManager::GetSupplies, "GetSupplies")
    .def("GetDemands", &EconomicActorManager::GetDemands, "GetDemands")
    .def("GetPopulation", &EconomicActorManager::GetPopulation, "GetPopulation");


  
  return m.ptr();
}



