

#include "PersonWithNeural.hh"
#include "FeedFowardNeuralNetImp.hh"
#include "EconomicActorManager.hh"
#include "MarketManager.hh"
#include "Company.hh"
#include "ActorLogger.hh"
#include "Calendar.hh"
PersonWithNeural::PersonWithNeural(EconomicActorManager *man ) : EconomicActor(man){

  //Set the persons brain to the FeedFoward neutron network implementation
  rBrain = new FeedFowardNeuralNetImp();

  rBrain->AddInput("FoodDemand",&fGoodDemandLevels[0]);
  rBrain->AddInput("OtherDemand",&fGoodDemandLevels[1]);

  rBrain->AddAction("BuyFood",[this](){
      OrderInfo info;
      int seller=this->fTheEconomicActorManager->GetMarketManager()->GetCheapestSeller(0,info);

      if (seller !=-1){
	int amount=std::min(this->fDemands[0].GetNumberOfCopies(),info.Quantity);
	double totalCost=info.Price*amount;
	
	if (totalCost <=this->fMoney){
	  rMessage<<"I am buying "<<info.Quantity<<" of good 0 for "<<totalCost<<" from "<<seller<<endl;
	  this->rDoTransaction(0,amount,info.Price,seller);
	}
      }
	
    } );

  rBrain->Train();
  fMoney=100000;
  AddSupply(0,100);
}

void PersonWithNeural::rDoTransaction(int Good2Buy,int AmountOfGoodIWant,double price,int Seller){
  double totalCost=(AmountOfGoodIWant)*price;
  //Move the money
  Company* theSeller =fTheEconomicActorManager->FindCompany(Seller);
  
  theSeller->AddMoney(totalCost);
  this->SubtractMoney(totalCost);

  theSeller->RemoveSupply(Good2Buy,AmountOfGoodIWant);
  this->AddSupply(Good2Buy,AmountOfGoodIWant);
  this->RemoveDemand(Good2Buy,AmountOfGoodIWant);


  fTheEconomicActorManager->GetMarketManager()->CleanUpOrder(Good2Buy,price,Seller,AmountOfGoodIWant);

}




PersonWithNeural::~PersonWithNeural(){
  delete rBrain;
}

ActorActions PersonWithNeural::BeginningOfStep(){
  rMessage.str("");
  rMessage<<"HI from begining of step"<<endl;
      
  AddDemand(0,10);

  //Log the information
  if (fMyActorLogger!=NULL){
    fMyActorLogger->LogBeforeStepState(this);
    fMyActorLogger->BeforeMessage(rMessage.str());
  }

  return ActorActions::None;
}

void PersonWithNeural::DoStep(){
  rMessage.str("");
  rMessage<<"HI from do step it is day "<<Calendar::DayNumber<<endl;;


  CalculateDemandLevels();
  rBrain->Think();

  
  if (fMyActorLogger!=NULL){
    fMyActorLogger->DuringMessage(rMessage.str());
  }


}


ActorActions PersonWithNeural::EndOfStep(){
  rMessage.str("");
  rMessage<<"HI From end of step"<<endl;

  ActorActions ret=ActorActions::None;

  cout<<"HI from guy "<<Calendar::DayNumber<<endl;
  if (Calendar::DayNumber > 500){

    rMessage<<"I have been smotten"<<endl;
    fTheEconomicActorManager->MarkForDeath(this);
    ret= ActorActions::Died;
  }


  if (fSupplies[0].GetNumberOfCopies() < 1&&
      Settings::CanStarveToDeath){
    rMessage<<"I have died :( "<<endl;
    
    //KillActor does not delete it right away.
    //it removes it from the 
    fTheEconomicActorManager->MarkForDeath(this);
    // for (auto i : rOwnedCompanies){
    //   fTheEconomicActorManager->MarkForDeath(i);
    // }//end for
    
    ret=ActorActions::Died;

  }else {
    fSupplies[0].RemoveCopies(1);
    rMessage<<"I ate "<<1<<" foods "<<endl;
    ret=ActorActions::None;
  }

  if (fMyActorLogger!=NULL){
    fMyActorLogger->LogAfterStepState(this);
    fMyActorLogger->EndMessage(rMessage.str());
  }

  return ret;
}
