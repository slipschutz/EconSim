#ifndef __PERSONTRAITPACT_HH
#define __PERSONTRAITPACT_HH






#include "TraitPact.hh"

class PersonTraitPact : public TraitPact
{
public:
  PersonTraitPact(){}
  virtual ~PersonTraitPact(){}

  virtual void InitializeTraits();

  void MutateCopy(const PersonTraitPact &);

  static float TraitMutationRate;
  static int DiscreteTraitMutationRate;

  static bool RunTests();
  
  float Restlessness;
  int Gluttoness;
  int FoodBuyingThreshold;
  int Food2BuyAtOnce;
  float InvestmentLevel;
  
};


#endif /* __PERSONTRAITPACT_HH */
