



#include "Manufacturer.hh"

#include "RandomManager.hh"

#include "Settings.hh"

#include "MarketManager.hh"
#include "GoodManager.hh"
#include "Person.hh"
#include "Good.hh"
#include "EconomicActorManager.hh"
#include "Death.hh"

void Manufacturer::Initialize(){


  //Pick a good to manufacturer
  GoodToManufacture=RandomManager::GetRand(Settings::MaxGoodNumber);
  
  //Randomly assign some attributes
  Conservativeness=RandomManager::GetRand(1000)/1000.0;
  Steadfastness=RandomManager::GetRand(1000)/1000.0;

  MaxVolume = RandomManager::GetRand(500)+10;///CAN"T BE 0

  rTotalVolumeCreated=0;

  for (int i=0;i<fGoodPriorities.size();i++){
    fGoodPriorities[i]=RandomManager::GetRand(Settings::MaxGoodPriority);
  }

  fSupplies.clear();
  fDemands.clear();

  rStartingSalary=RandomManager::GetRand(100);
}


ActorActions Manufacturer::BeginningOfStep(){


  rStartOfStepMoney=fMoney;

  ////////////////////////////////////////////////////////////
  // if the company has employees it can manufacture goods  //
  ////////////////////////////////////////////////////////////
    
  if ( Employees2Salary.size() > 0 ){
    if (fSupplies.find(GoodToManufacture) == fSupplies.end()){
      //Good isn't there yet
      //Only manufacturers should be able to make goods

      int t=RandomManager::GetRand(MaxVolume);
      Good temp(GoodToManufacture,t,fGoodPriorities[GoodToManufacture],"supply");
      fSupplies[GoodToManufacture]=temp;
      rTotalVolumeCreated+=t;
    } else {
      int AmountOfGood=fSupplies[GoodToManufacture].GetNumberOfCopies();
      int t=RandomManager::GetRand(MaxVolume);
      fSupplies[GoodToManufacture].AddCopies(t);
      rTotalVolumeCreated+=t;
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

  return ActorActions::None;
}


ActorActions Manufacturer::EndOfStep(){

  //Compare how much was earned to how much needs to be payed to employees
  double thisStepProfit = fMoney-rStartOfStepMoney;
  if (thisStepProfit >0 ){//Made money
    //give 10% to the owner of the company
    if (fTheOwner !=NULL){
      fTheOwner->GetPaid(0.1*thisStepProfit,"Receiving returns from owning company");
    }
    this->SubtractMoney(0.1*thisStepProfit);
  }else {//We are not making moeny


  }

  ActorActions ret=ActorActions::None;

  //Pay the employees if able
  for (auto & i : Employees2Salary){
    double moneyToPay=i.second;
    if (moneyToPay > fMoney){
      //Can't pay that going bankrupt
      fTheEconomicActorManager->MarkForDeath(this);
      ret=ActorActions::Died;
      break;
    }else {
      stringstream stemp;stemp<<"Company "<<GetBaseId()<<" is paying salary "<<moneyToPay<<endl;
      i.first->GetPaid(moneyToPay,stemp.str());
      this->SubtractMoney(moneyToPay);
    }
  }
  
  return ret;
}


void Manufacturer::DumpConnections(){
  cout<<"Connections For Manufacturer "<<this->GetBaseId()<<endl;
  for (auto & ii : fConnections){
    cout<<"    Manufacturer "<<this->GetBaseId()<<" connects to "<<ii.first<<endl;
  }


}

void Manufacturer::PrintInfo(){
  PrintLine('v',30);
  cout<<"Info for <Manufacturer> base id "<<this->GetBaseId()<<" actor type "<<this->GetActorType()<<endl;
  cout<<"I have "<<Employees2Salary.size()<<" employees.  I make "<<GoodToManufacture<<endl;
  cout<<"My Maximum volume is "<<MaxVolume<<" I have made "<<rTotalVolumeCreated<<" total"<<endl;
  cout<<"I have "<<fMoney<<" monies"<<endl;
  PrintLine('^',30);
}
