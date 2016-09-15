



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
  rConservativeness=RandomManager::GetRand(1000)/1000.0;
  rSteadfastness=0.1;//RandomManager::GetRand(1000)/1000.0;

  MaxVolume = RandomManager::GetRand(500)+10;///CAN"T BE 0
  rPriceChangeLevel= RandomManager::GetRand(100)/100.;
  


  for (int i=0;i<fGoodPriorities.size();i++){
    fGoodPriorities[i]=41;//RandomManager::GetRand(Settings::MaxGoodPriority);
  }

  fSupplies.clear();
    fDemands.clear();

  rStartingSalary=RandomManager::GetRand(100);


  rTotalVolumeCreated=0;
  rStartOfStepMoney=0;
  rStartOfStepSupply=0;

  rFirstStep=true;


  numberOfSteps=0;
  numberOFProductions=0;
  
}


ActorActions Manufacturer::BeginningOfStep(){


  rStartOfStepMoney=fMoney;
  

  ////////////////////////////////////////////////////////////
  // if the company has employees it can manufacture goods  //
  ////////////////////////////////////////////////////////////
  if (fSupplies.find(GoodToManufacture) == fSupplies.end()){
    Good temp(GoodToManufacture,0,fGoodPriorities[GoodToManufacture],"supply");
    fSupplies[GoodToManufacture]=temp;
  }
    
  if ( Employees2Salary.size() > 5){
    int t=Employees2Salary.size()*10;
    fSupplies[GoodToManufacture].AddCopies(t);
    rTotalVolumeCreated+=t;

    numberOFProductions++;
  }
  
  if ( fSupplies[GoodToManufacture].GetNumberOfCopies()<0){
    cout<<"Less than zero"<<endl;
    throw 1;
  }
  
  
  if (fSupplies.size() !=0 && fSupplies[GoodToManufacture].GetNumberOfCopies()!=0){
    
    MarketManager::Get()->PlaceSellOrder(GoodToManufacture,this->GetBaseId(),
					 fSupplies[GoodToManufacture].GetNumberOfCopies(),
					 fGoodPriorities[GoodToManufacture]);
  }
  


  for (int i=0;i<10-Employees2Salary.size();i++){
    MarketManager::Get()->PlaceJobPosting(rStartingSalary,this->GetBaseId());
  }


  auto it_temp=fSupplies.find(GoodToManufacture);
  if (it_temp != fSupplies.end()){
    rStartOfStepSupply=it_temp->second.GetNumberOfCopies();
  }else{
    rStartOfStepSupply=0;
  }

  return ActorActions::None;
}


ActorActions Manufacturer::EndOfStep(){

  //Compare how much was earned to how much needs to be payed to employees
  double thisStepProfit = fMoney-rStartOfStepMoney;
  double thisStepSoldVolume=0;
  
  auto it_temp=fSupplies.find(GoodToManufacture);
  if (it_temp != fSupplies.end()){
    thisStepSoldVolume=rStartOfStepSupply-it_temp->second.GetNumberOfCopies();
  }



  if (thisStepProfit >0 ){//Made money
    //give 10% to the owner of the company
    // if (fTheOwner !=NULL){
    //   fTheOwner->GetPaid(0.1*thisStepProfit,"Receiving returns from owning company");
    //   this->SubtractMoney(0.1*thisStepProfit);
    // }

  }else {//We are not making moeny

  }

  if (RandomManager::GetRand(100)/100. > rSteadfastness){
    if (thisStepSoldVolume < 10){
      int temp =fGoodPriorities[GoodToManufacture];
      // cout<<"before price "<<temp<<endl;
      temp = temp - temp*rPriceChangeLevel;
      // cout<<"after price "<<temp<<endl;
      fGoodPriorities[GoodToManufacture]=temp;

      // cout<<"This is manufacturer "<<this->GetBaseId()<<" in lower price"<<endl;
      // cout<<"Steadfastneess "<<rSteadfastness<<endl;
      // cout<<"Volume sold "<<thisStepSoldVolume<<endl;
      // cout<<"rPriceChangelevel "<<rPriceChangeLevel<<endl;
      // cin.get();

    }else if (thisStepSoldVolume > 0.85*rStartOfStepSupply){
      int temp =fGoodPriorities[GoodToManufacture];
      // cout<<"before price "<<temp<<endl;
      temp = temp + temp*rPriceChangeLevel;
      // cout<<"after price "<<temp<<endl;
      fGoodPriorities[GoodToManufacture]=temp;
      // cout<<"This is manufacturer "<<this->GetBaseId()<<" in raise price"<<endl;
      // cout<<"Steadfastneess "<<rSteadfastness<<endl;
      // cout<<"Volume sold "<<thisStepSoldVolume<<endl;
      // cout<<"rPriceChangelevel "<<rPriceChangeLevel<<endl;
      // cin.get();
   
    }
  }
  ActorActions ret=ActorActions::None;

  //Pay the employees if able
  for (auto & i : Employees2Salary){
    double moneyToPay=i.second;
    if (moneyToPay > fMoney){
      // cout<<"Comapny "<<this->GetBaseId()<<" going bankrupt "<<endl;
      // cout<<"I have this number of employeees "<<Employees2Salary.size()<<endl;
      // for (auto j : Employees2Salary){
      // 	cout<<"Employee "<<j.first<<" pay "<<j.second<<endl;
      // }
      // cout<<"Num productions "<<numberOFProductions<<endl;
      // cout<<"steps "<<numberOfSteps<<endl;
      // cin.get();
 
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

  numberOfSteps++;
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
