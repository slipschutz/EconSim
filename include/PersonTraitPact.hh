#ifndef __PERSONTRAITPACT_HH
#define __PERSONTRAITPACT_HH






#include "TraitPact.hh"

class PersonTraitPact : public TraitPact
{
public:
  virtual ~PersonTraitPact(){}
  virtual void InitializeTraits();

  void MutateCopy(const PersonTraitPact &);

  static float TraitMutationRate;
  static int DiscreteTraitMutationRate;

  static bool RunTests();

  PersonTraitPact(){
    // theTraitPact.AddTrait(& Restlessness);
    // theTraitPact.AddTrait(& Gluttoness);
    // theTraitPact.AddTrait(& FoodBuyingThreshold);
    // theTraitPact.AddTrait(& Food2BuyAtOnce);
    // theTraitPact.AddTrait(& InvestmentLevel);
  }
  
  float Restlessness;
  int Gluttoness;
  int FoodBuyingThreshold;
  int Food2BuyAtOnce;
  float InvestmentLevel;


};


#endif /* __PERSONTRAITPACT_HH */
