

#include "ManufacturerTraitPact.hh"
#include "RandomManager.hh"

float ManufacturerTraitPact::TraitMutationRate=0.05;

ManufacturerTraitPact::ManufacturerTraitPact(){

}

ManufacturerTraitPact::~ManufacturerTraitPact(){
  
}

void ManufacturerTraitPact::InitializeTraits(){

  Conservativeness=RandomManager::GetUniform();
  Steadfastness=RandomManager::GetUniform();
  PriceChangeLevel=RandomManager::GetUniform();
  StartingSalary=RandomManager::GetUniform(0,10000);


}



void ManufacturerTraitPact::MutateCopy(const ManufacturerTraitPact & RHS){

  Conservativeness=RandomManager::GetUniform();
  Steadfastness   =RandomManager::GetUniform();
  PriceChangeLevel=RandomManager::GetUniform();
  StartingSalary  =RandomManager::GetUniform(0,10000);


}
