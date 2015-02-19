



#include "Manufacturer.hh"

#include "RandomManager.hh"

#include "Settings.hh"

#include "MarketManager.hh"
#include "GoodManager.hh"
#include "Person.hh"
#include "Good.hh"
void Manufacturer::Initialize(){
  fMoney=10000;

  //Pick a good to manufacturer
  GoodToManufacture=RandomManager::GetRand(Settings::MaxGoodNumber);
  
  //Randomly assign some attributes
  Conservativeness=RandomManager::GetRand(1000)/1000.0;
  Steadfastness=RandomManager::GetRand(1000)/1000.0;

  MaxVolume = RandomManager::GetRand(5)+1000;///CAN"T BE 0

  for (int i=0;i<fGoodPriorities.size();i++){
    fGoodPriorities[i]=RandomManager::GetRand(Settings::MaxGoodPriority);
  }

  fSupplies.clear();
  fDemands.clear();

  rStartingSalary=RandomManager::GetRand(100);
}


void Manufacturer::BeginningOfStep(){

  ////////////////////////////////////////////////////////////
  // if the company has employees it can manufacture goods  //
  ////////////////////////////////////////////////////////////
  
  
  
  if ( Employees2Salary.size() >10 ){
    
    if (fSupplies.find(GoodToManufacture) == fSupplies.end()){
      //Good isn't there yet
      //Only manufacturers should be able to make goods

      int t=RandomManager::GetRand(MaxVolume);
      Good temp(GoodToManufacture,t,fGoodPriorities[GoodToManufacture],"supply");
      fSupplies[GoodToManufacture]=temp;
      
    } else {
      int AmountOfGood=fSupplies[GoodToManufacture].GetNumberOfCopies();
      int t=RandomManager::GetRand(MaxVolume);
      fSupplies[GoodToManufacture].AddCopies(t);
      
    }
  
    if ( fSupplies[GoodToManufacture].GetNumberOfCopies()<0){
      cout<<"Less than zero"<<endl;
      throw 1;
    }
    if (fSupplies.size() !=0 && fSupplies[GoodToManufacture].GetNumberOfCopies()!=0){
      MarketManager::Get()->PlaceSellOrder(GoodToManufacture,this->GetBaseId(),
					   fSupplies[GoodToManufacture].GetNumberOfCopies(),
					   fSupplies[GoodToManufacture].GetNormPriority()*100);
    }
  } else { //Try to hire people
    for (int i=0;i<10-Employees2Salary.size();i++){
      MarketManager::Get()->PlaceJobPosting(rStartingSalary,this->GetBaseId());
    }
  }
}


bool Manufacturer::EndOfStep(){
  //Pay the employees
  bool GoingBankrupt=false;
  for (auto & i : Employees2Salary ){
    double moneyToPay=i.second;
    if (moneyToPay > fMoney){
      //Can't pay that going bankrupt
      GoingBankrupt=true;
    }else {
      i.first->AddMoney(moneyToPay);
      this->SubtractMoney(moneyToPay);
    }
  }
  
  if(GoingBankrupt){
    for (auto & i : Employees2Salary){
      i.first->YourFired();
    }
    
    return true;
  }else{
    return false;
  }
}


void Manufacturer::DumpConnections(){
  cout<<"Connections For Manufacturer "<<this->GetBaseId()<<endl;
  for (auto & ii : fConnections){
    cout<<"    Manufacturer "<<this->GetBaseId()<<" connects to "<<ii.first<<endl;
  }


}
