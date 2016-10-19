

#include "PersonTraitPact.hh"
#include "RandomManager.hh"
#include "Utilities.hh"


float PersonTraitPact::TraitMutationRate=0.05;
int PersonTraitPact::DiscreteTraitMutationRate=2;

bool PersonTraitPact::RunTests(){
  bool status=true;

  PersonTraitPact testPact;
  testPact.InitializeTraits();

  status=DoTest(testPact.Restlessness >0 &&testPact.Restlessness<1,"Restlessness intializes correctly");
  status=DoTest(testPact.InvestmentLevel >0 &&testPact.Restlessness<1,"Investmentlevel intializes correctly");

  status=DoTest(testPact.Gluttoness >0,"Gluttoness inializes correctly");
  status=DoTest(testPact.FoodBuyingThreshold >0,"ttoness inializes correctly");
  status=DoTest(testPact.Gluttoness >0,"Gluttoness inializes correctly");
  
  return status;
}

void PersonTraitPact::InitializeTraits(){

  Restlessness=RandomManager::GetUniform();
  Gluttoness=RandomManager::GetRand(10)+1;
  FoodBuyingThreshold=RandomManager::GetRand(100)+10;
  Food2BuyAtOnce=RandomManager::GetRand(100)+10;
  InvestmentLevel=RandomManager::GetUniform();

}

void PersonTraitPact::MutateCopy(const PersonTraitPact & RHS){


  Gluttoness =RHS.Gluttoness + RandomManager::GetRand(DiscreteTraitMutationRate) -
    DiscreteTraitMutationRate/2;
  FoodBuyingThreshold =RHS.FoodBuyingThreshold+ RandomManager::GetRand(DiscreteTraitMutationRate) -
    DiscreteTraitMutationRate/2;
  Food2BuyAtOnce      =RHS.Food2BuyAtOnce+ RandomManager::GetRand(DiscreteTraitMutationRate) -
    DiscreteTraitMutationRate/2;


  InvestmentLevel     =RHS.InvestmentLevel + RandomManager::GetGaus(0,0.05);
  Restlessness        =RHS.Restlessness + RandomManager::GetGaus(0,TraitMutationRate);


  BoundAboveZero(Gluttoness);
  BoundAboveZero(FoodBuyingThreshold);
  BoundAboveZero(Food2BuyAtOnce);

  BoundZeroToOne(InvestmentLevel);
  BoundZeroToOne(Restlessness);
  
    
}
