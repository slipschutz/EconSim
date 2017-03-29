


#include "Manufacturer.hh"

#include "RandomManager.hh"

#include "Settings.hh"
#include "Calendar.hh"
#include "MarketManager.hh"
#include "GoodManager.hh"
#include "Person.hh"
#include "Good.hh"
#include "EconomicActorManager.hh"
#include "Death.hh"
#include "Calendar.hh"
#include "ActorLogger.hh"



void Manufacturer::Initialize(){



  //Pick a good to manufacturer
  //  GoodToManufacture=RandomManager::GetRand(Settings::MaxGoodNumber);
  
  //Randomly assign some attributes
  rConservativeness=RandomManager::GetRand(1000)/1000.0;
  rSteadfastness=RandomManager::GetRand(1000)/2000.0 + 0.5;

  MaxVolume = RandomManager::GetRand(500)+10;///CAN"T BE 0
  rPriceChangeLevel= RandomManager::GetRand(100)/100.;

  rSalaryChangeLikelyhood=RandomManager::GetUniform();
  
  int temp=floor(0.1*fMoney);
  if (temp ==0){
    temp=1;
  }
  rStartingSalary=RandomManager::GetRand(temp)+100;


  for (unsigned int i=0;i<fGoodPriorities.size();i++){
    fGoodPriorities[i]=41;//RandomManager::GetRand(Settings::MaxGoodPriority);
  }

  fSupplies.clear();
  fDemands.clear();




  rTotalVolumeCreated=0;
  rStartOfStepMoney=0;
  rStartOfStepSupply=0;

  rFirstStep=true;


  numberOfSteps=0;
  numberOFProductions=0;
  

  rGoodPrice=RandomManager::GetRand(1000);
  
  rHireMorePeople=true;
}


ActorActions Manufacturer::BeginningOfStep(){
  //Save the start of step money
  rStartOfStepMoney=fMoney;


  ////////////////////////////////////////////////////////////
  // if the company has employees it can manufacture goods  //
  ////////////////////////////////////////////////////////////
  if (fSupplies.find(GoodToManufacture) == fSupplies.end()){
    Good temp(GoodToManufacture,0,fGoodPriorities[GoodToManufacture],GoodType::Supply);
    fSupplies[GoodToManufacture]=temp;
  }

    
  //Make the good based on the number of empoloyees that are employeed
  //  if ( Calendar::DayNumber < 1200){
    int t=Employees2Salary.size();
    fSupplies[GoodToManufacture].AddCopies(floor(t*Settings::FoodProductionPerWorker));
    rTotalVolumeCreated+=t;
    numberOFProductions++;
    //  }
  

  //Put sell orders on to market  
  if (fSupplies.size() !=0 && fSupplies[GoodToManufacture].GetNumberOfCopies()!=0){
    
    fTheEconomicActorManager->GetMarketManager()->PlaceSellOrder(GoodToManufacture,this->GetBaseId(),
								 fSupplies[GoodToManufacture].GetNumberOfCopies(),
								 rGoodPrice);/////
  }
  

  //Hire more people if in the last steps EndOfStep set HireMorePeople to true
  if (rHireMorePeople || Employees2Salary.size()==0){
    for (int i=0;i<5;i++){
      fTheEconomicActorManager->GetMarketManager()->PlaceJobPosting(rStartingSalary,this->GetBaseId());
    }
    rHireMorePeople=false;
  }


  auto it_temp=fSupplies.find(GoodToManufacture);
  if (it_temp != fSupplies.end()){
    rStartOfStepSupply=it_temp->second.GetNumberOfCopies();
  }else{
    rStartOfStepSupply=0;
  }


  if (fMyActorLogger!=NULL){
    stringstream ss;
    ss<<"Hello from manufacturer today is "<<Calendar::DayNumber<<endl;
    fMyActorLogger->BeforeMessage(ss.str());
    fMyActorLogger->LogBeforeStepState(this);
  }


  return ActorActions::None;
}

void Manufacturer::DoStep(){
  if (fMyActorLogger!=NULL){
    fMyActorLogger->DuringMessage("Durring Message");
  }
 }
ActorActions Manufacturer::EndOfStep(){
  stringstream ss;
   

  
  //See how much money was made during the step
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
    rHireMorePeople=true;
    ss<<"I made money so I am gonna try to hire more people"<<endl;
  }else {//We are not making moeny
    if (RandomManager::GetUniform() < 0.1 && Employees2Salary.size()!=0){
      //
      auto it= Employees2Salary.begin();
      ss<<"I am not making any money I am going to fire "<<it->first->GetBaseId()<<endl;
      it->first->YourFired();
      Employees2Salary.erase(it);
    }
    
  }


  if (RandomManager::GetUniform() < rSalaryChangeLikelyhood){
    ss<<"I am changing my salary from "<<rStartingSalary<<" to ";
    rStartingSalary*=RandomManager::GetUniform(0,2);
    ss<<rStartingSalary<<endl;
  }

  
  if (RandomManager::GetUniform() > rSteadfastness){
    if (thisStepSoldVolume < 10){


      double oldPrice=rGoodPrice;
      rGoodPrice=oldPrice - oldPrice*rPriceChangeLevel;
      if (rGoodPrice<0){
	rGoodPrice=0;
      }



      // cout<<"This is manufacturer "<<this->GetBaseId()<<" in lower price"<<endl;
      // cout<<"Steadfastneess "<<rSteadfastness<<endl;
      // cout<<"Volume sold "<<thisStepSoldVolume<<endl;
      // cout<<"rPriceChangelevel "<<rPriceChangeLevel<<endl;
      // cin.get();


      ss<<"I am lowering my price for good "<<GoodToManufacture<<" it is "<<rGoodPrice<<endl;
      
    }else if (thisStepSoldVolume > 0.85*rStartOfStepSupply){

      double oldPrice=rGoodPrice;
      rGoodPrice=oldPrice +oldPrice*rPriceChangeLevel;
      if (rGoodPrice<0){
	rGoodPrice=0;
      }

      // cout<<"This is manufacturer "<<this->GetBaseId()<<" in raise price"<<endl;
      // cout<<"Steadfastneess "<<rSteadfastness<<endl;
      // cout<<"Volume sold "<<thisStepSoldVolume<<endl;
      // cout<<"rPriceChangelevel "<<rPriceChangeLevel<<endl;
      // cin.get();

      ss<<"I am raising my price for good "<<GoodToManufacture<<" it is "<<rGoodPrice<<endl;
    }
  }
  ActorActions ret=ActorActions::None;

  //Pay the employees if able
  for (auto & i : Employees2Salary){
    double moneyToPay=i.second;
    if (moneyToPay > fMoney){

      ss<<"Going Bankrupt. Trying to pay employee "<<i.first->GetBaseId()<<" "<<moneyToPay<<" but only have "<<fMoney<<"\n";
      
      fTheOwner->AddMoney(fMoney);
      this->SubtractMoney(fMoney);
      //Can't pay that going bankrupt
      fTheEconomicActorManager->MarkForDeath(this);
      ret=ActorActions::Died;
      break;
    }else {
      stringstream stemp;stemp<<"Company "<<GetBaseId()<<" is paying salary "<<moneyToPay<<endl;
      i.first->GetPaid(moneyToPay,stemp.str());

      this->SubtractMoney(moneyToPay);
      ss<<" I Paid an employee "<<i.first->GetBaseId()<<" "<<moneyToPay<<" I have "<<fMoney<<" money left"<<endl;
    }
  }



  if (fMyActorLogger!=NULL){
    fMyActorLogger->EndMessage(ss.str());
    fMyActorLogger->LogAfterStepState(this);
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
