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


vector<double> GetPriceData(){
  vector <TransactionRecord>* theGoodPrices=  DataLogger::Get()->GetThePrices();
  vector  <double> price;
  cout<<"The size is "<<theGoodPrices->size()<<endl;
  for ( auto i : *theGoodPrices){
    price.push_back(i.Price);
  }

  return price;
}


vector<double> GetSupplies(){

  return DataLogger::Get()->theSupplies;

}

vector<int> GetPopulation(){

  return DataLogger::Get()->GetNumberOfPeople();

}

vector<int> GetNumManufacturers(){

  return DataLogger::Get()->GetNumberOfManufacturers();

}


vector<double> GetDemands(){


  return DataLogger::Get()->theDemands;

}




/*
template<class T>
bp::list std_vector_to_py_list(const std::vector<T>& v)
{
  bp::list l;
  for ( auto & i : v){
    l.append(i);
  }
  return l;
}







*/

#include <assert.h>
#include <string>


#include "Utilities.hh"




void MoreTests(){

  EconomicActorManager * man =new EconomicActorManager();
  double totalMoney=0;
  Person *lucky;
  Settings::CanStarveToDeath=false;
  for (int i=0;i<10;i++){
    Person *aPerson1 = new Person(man);
    aPerson1->Initialize();
    man->MakeActor(aPerson1);
    totalMoney+=aPerson1->GetMoney();
    lucky=aPerson1;
  }

  for (int i=0;i<2;i++){
    totalMoney+=10000;
    Manufacturer * m = new Manufacturer(10000,man,lucky,0);
    m->SetActorLogger(new ActorLogger(m->GetBaseId()));
    man->MakeActor(m);
  }
  cout<<setprecision(20);


  for (int i=0;i<1000;i++){
    man->DoAStep();
    MarketManager::Get()->ClearMarket();
    Calendar::DayNumber++;
  }

  double endingMoney=0;
  for (auto i : *man->GetList()){
    endingMoney+=i.second->GetMoney();
  }

  DoTest(abs(endingMoney-totalMoney)<0.0000001,"Money should be conserved");
  delete man;
}



void UnitTests(){

  EconomicActorManager * man =new EconomicActorManager();


  //Basic People Tests
  Person aPerson1(man);
  man->MakeActor(&aPerson1);


  int totalSupply = (*aPerson1.GetSupplies())[0].GetNumberOfCopies();
  DoTest(GoodManager::Get()->supply[0]==totalSupply,"Total Supply should be the supply of person 1");

  Person aPerson2(man);
  man->MakeActor(&aPerson2);


  totalSupply += (*aPerson2.GetSupplies())[0].GetNumberOfCopies();
  DoTest(GoodManager::Get()->supply[0]==totalSupply,"Total Supply should be the supply of person 1 and 2");

  Manufacturer *aManufac= new Manufacturer(1000,man,&aPerson1,0);

  man->MakeActor(aManufac);
  DoTest(aManufac->GetMoney()==1000,"Manufacter should start with 1000 dollars");

  string aString="Company owner not the right person";

  DoTest(aManufac->GetTheOwner()==&aPerson1,aString);

  aManufac->AddEmployee(&aPerson2,100);

  DoTest(aManufac->GetNumberOfEmployees()==1,"Company Should Have 1 Employee");


  int totalAmtForManu=25;
  totalSupply += totalAmtForManu;

  aManufac->SetGoodPriority(0,50);//Set good 0 to proity 50 (should mean price=50)
  aManufac->AddSupply(0,totalAmtForManu);


  DoTest(GoodManager::Get()->supply[0]==totalSupply,"Total Supply of good 0 equals person1+person2+25");

  
  Person aPerson3(man);
  man->MakeActor(&aPerson3); 
  totalSupply += (*aPerson3.GetSupplies())[0].GetNumberOfCopies();

  DoTest(GoodManager::Get()->supply[0]==totalSupply,"Total Supply of good 0 equals person1+person2+25+person3");

  aPerson3.SetRestlessnes    (0);
  aPerson3.SetGluttoness     (0);
  aPerson3.SetInvestmentLevel(0);
  aPerson3.AddDemand(0,10);

  DoTest(10==GoodManager::Get()->demand[0],"Total demand should be 10");


  aPerson3.BeginningOfStep();


  MarketManager::Get()->PlaceSellOrder(0,aManufac->GetBaseId(),
				       totalAmtForManu,aManufac->GetGoodPriority(0)); 


  auto goods4SaleBefore =*(MarketManager::Get()->GetCurrentGoodsForSale());


  DoTest(int(goods4SaleBefore.size())==Settings::MaxGoodNumber,"The goods for sale vector in market manager should be of length Settings::MaxGoodNumber");

  DoTest(goods4SaleBefore.at(0)==25,"There should be 25 of good 0 still left for sale before any sales have happened");
				   
  double preStepMoney=aPerson3.GetMoney();
  aPerson3.DoStep();

  auto goods4Sale =*(MarketManager::Get()->GetCurrentGoodsForSale());
  DoTest(goods4Sale.at(0)==15,"There should be 25-10 of good 0 still left for sale");
  



  DoTest(aPerson3.GetMoney()==preStepMoney-10*50,"Person3 should have lost money equal to the transaction");

  DoTest(totalSupply==GoodManager::Get()->supply[0],"Total supply should be the same");

  DoTest(0==GoodManager::Get()->demand[0],"Total demand should be 0");

  

  aPerson2.SetRestlessnes    (0);
  aPerson2.SetGluttoness     (0);
  aPerson2.SetInvestmentLevel(0);
  aPerson2.AddDemand(0,50);

  double preStepPerson2=aPerson2.GetMoney();
  aPerson2.DoStep();
  //There should be 15 units of 0 left on the market
  DoTest(totalSupply==GoodManager::Get()->supply[0],"Total supply should be the same");
  DoTest(35==GoodManager::Get()->demand[0],"Total demand should be 35");

  DoTest(preStepPerson2-15*50==aPerson2.GetMoney(),"Person2 should have lost 15*50 dollars");


  DoTest(50*25+1000 == aManufac->GetMoney(),"Manufacturer should now have the money from person2,3");

  aManufac->EndOfStep(); 

  DoTest(preStepPerson2-15*50 + 100==aPerson2.GetMoney(),"Person2 should have now gotten paid 100");



  Person aPerson4(man);
  aPerson4.Initialize();
  DoTest(aPerson4.GetHasJob()==false,"Person should not have a job yet");
  MarketManager::Get()->PlaceJobPosting(10,aManufac->GetBaseId());
  JobInfo jInfo= MarketManager::Get()->GetBestJob();
  DoTest(jInfo.EmployerID!=-1,"There should be a job available on the market");
  aPerson4.DoStep();
  DoTest(aPerson4.GetHasJob()==true,"Person should Now have a job");
  DoTest(aPerson4.GetEmployer()->GetBaseId()==aManufac->GetBaseId(),"The company* inside of person should be correct");

  double moneyPerson4Orig=aPerson4.GetMoney();
  double moneyManuOrig =aManufac->GetMoney();
  aManufac->EndOfStep();
  DoTest(moneyPerson4Orig+10 ==aPerson4.GetMoney(),"Person4 should have been paid 10");

  DoTest(moneyManuOrig-10-100 == aManufac->GetMoney(),"manufac should have give 10 money away to person4 and a 100 to person2");

  
  DoTest((*aManufac->GetSupplies())[0].GetNumberOfCopies() == 0,"The Manufacturer should have no supply left");

}


PYBIND11_PLUGIN(PyEconSim) {
  py::module m("PyEconSim", "pybind11 example plugin");

  m.def("Run", &Run, "Run Simulation");
  m.def("DoEndOfDay", &DoEndOfDay, "Clean Up");
  m.def("UnitTests", &UnitTests, "Run Tests");
  m.def("GetPriceData", &GetPriceData, "GetPriceData");
  m.def("GetSupplies", &GetSupplies, "GetSupplies");
  m.def("GetDemands", &GetDemands, "GetDemands");
  m.def("GetPopulation", &GetPopulation, "GetPopulation");

  
  py::class_<EconomicActorManager>(m, "EconomicActorManager")
    .def(py::init<>())
    .def("DoAStep", &EconomicActorManager::DoAStep)
    .def("BuildCompleteNetwork", &EconomicActorManager::BuildCompleteNetwork);
  
  
  return m.ptr();
}



