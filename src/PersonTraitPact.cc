

#include "PersonTraitPact.hh"
#include "RandomManager.hh"


void PersonTraitPact::InitializeTraits(){

  Restlessness=RandomManager::GetUniform();
  Gluttoness=RandomManager::GetRand(10)+1;
  FoodBuyingThreshold=RandomManager::GetRand(100)+10;
  Food2BuyAtOnce=RandomManager::GetRand(100)+10;
  InvestmentLevel=RandomManager::GetUniform();

}

void PersonTraitPact::MutateCopy(const PersonTraitPact & RHS){

  Restlessness        =RHS.Restlessness + RandomManager::GetGaus(0,0.05);
  Gluttoness          =RHS.Gluttoness;
  FoodBuyingThreshold =RHS.FoodBuyingThreshold;
  Food2BuyAtOnce      =RHS.Food2BuyAtOnce;
  InvestmentLevel     =RHS.InvestmentLevel + RandomManager::GetGaus(0,0.05);


}
