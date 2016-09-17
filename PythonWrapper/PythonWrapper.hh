

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

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

using namespace std;





namespace bp = boost::python;

template<class T>
bp::list std_vector_to_py_list(const std::vector<T>& v)
{
  bp::list l;
  for ( auto & i : v){
    l.append(i);
  }
  return l;
}


bp::list GetData(){

  vector <TransactionRecord>* theGoodPrices=  DataLogger::Get()->GetThePrices();
  
  vector  <double> price;
  cout<<"The size is "<<theGoodPrices->size()<<endl;
  for ( auto i : *theGoodPrices){
    price.push_back(i.Price);
  }

  // vector <double> * prices = DataLogger::Get()->GetTheSalaries();

  return std_vector_to_py_list(price);

}

bp::list GetSupplies(){


  return std_vector_to_py_list(DataLogger::Get()->theSupplies);

}

bp::list GetDemands(){


  return std_vector_to_py_list(DataLogger::Get()->theDemands);

}


void Run(){

  try{
    EconomicActorManager *  theManager = new EconomicActorManager();
    
    DataLogger::Get();
    
    theManager->BuildCompleteNetwork(Settings::NumberOfPeople);
  
    cout<<"BUILT NETWORK"<<endl;

  
    for (int i=0;i<Settings::NumberOfSteps;i++){
    
      theManager->DoAStep();
      // MarketManager::Get()->Dump();
      // cin.get();
      if (i%10==0){
	cout<<"ON "<<i<<endl;
	// cout<<"VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV"<<endl;
	// GoodManager::Get()->Dump();
	// MarketManager::Get()->Dump();
	// cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl;
	// cin.get();

	
      }
      if (i==Settings::NumberOfSteps-1){
	// cout<<"\n\n\n";
	// GoodManager::Get()->Dump();
	
	// cin.get();
      }
      DataLogger::Get()->LogMarketState(MarketManager::Get(), GoodManager::Get());

      MarketManager::Get()->ClearMarket();
      Calendar::DayNumber++;

    }


 
    

    //theManager.PrintMoney();
    // theManager.PrintHavesWants();
    DataLogger::Get()->LogEndingMoneyDistribution(theManager->GetList());

    delete theManager;
    //    GoodManager::Get()->Dump();
    ActorLogger::Get()->DumpLog();


    // if (DataLogger::Get() != NULL){
    //   delete DataLogger::Get();
    // }
  } catch (exception &e){
    cout<<"*******************************************************"<<endl;
    cout<<"Fatal Exception Must exit"<<endl;
    cout<<e.what()<<endl;
    cout<<"*******************************************************"<<endl;
  }
  


}

#include <assert.h>
#include <string>


void DoTest(bool test,string message){

  if(!test){
    cout<<"*******Test Fail********* "<<message<<endl;
  }else{
    cout<<"Passed "<<message<<endl;
  }

}


void UnitTests(){
  /*
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

  Manufacturer *aManufac= new Manufacturer(1000,man,&aPerson1);

  man->MakeActor(aManufac);


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
				   
  double preStepMoney=aPerson3.GetMoney();
  aPerson3.DoStep();
  
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
  */  
}




BOOST_PYTHON_MODULE(libWrapper){
  bp::def("Run",Run);
  bp::def("GetData",GetData);
  bp::def("GetSupplies",GetSupplies);
  bp::def("GetDemands",GetDemands);

  bp::def("UnitTests",UnitTests);
  

  //  bp::class_<std::vector<double> > ("AVec").def(bp::vector_indexing_suite<std::vector<double> >());

}