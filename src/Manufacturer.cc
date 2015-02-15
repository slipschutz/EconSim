



#include "Manufacturer.hh"

#include "RandomManager.hh"

#include "Settings.hh"

#include "MarketManager.hh"
#include "GoodManager.hh"

#include "Good.hh"
void Manufacturer::Initialize(){
  

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
}


void Manufacturer::BeginningOfStep(){

  ////////////////////////////////////////////////////////////
  // if the company has employees it can manufacture goods  //
  ////////////////////////////////////////////////////////////
  
  if ( Employees2Salary.size() == 0 ){
    
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
  }
  if ( fSupplies[GoodToManufacture].GetNumberOfCopies()<0){
    cout<<"Less than zero"<<endl;

    throw 1;
  }

  //Now post something to the market.  As first test always post all the goods that company has

  //  cout<<"THIS IS SUPPLIES "<<fSupplies[GoodToManufacture].GetNumberOfCopies()<<endl;


  if (fSupplies.size() !=0 && fSupplies[GoodToManufacture].GetNumberOfCopies()!=0){
    MarketManager::Get()->PlaceSellOrder(GoodToManufacture,this->GetBaseId(),
					 fSupplies[GoodToManufacture].GetNumberOfCopies(),
					 fSupplies[GoodToManufacture].GetNormPriority());
  }
  
}





void Manufacturer::DumpConnections(){
  cout<<"Connections For Manufacturer "<<this->GetBaseId()<<endl;
  for (auto & ii : fConnections){
    cout<<"    Manufacturer "<<this->GetBaseId()<<" connects to "<<ii.first<<endl;
  }


}
