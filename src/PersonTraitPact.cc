

#include "PersonTraitPact.hh"
#include "RandomManager.hh"


void PersonTraitPact::InitializeTraits(){

  Restlessness=RandomManager::GetUniform();
  Gluttoness=RandomManager::GetUniform();
  FoodBuyingThreshold=RandomManager::GetUniform();
  Food2BuyAtOnce=RandomManager::GetUniform();
  InvestmentLevel=RandomManager::GetUniform();

}


// bool PersonTraitPact::TestTrait(float *address){



// }
