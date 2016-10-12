#ifndef __PERSONTRAITPACT_HH
#define __PERSONTRAITPACT_HH






#include "TraitPact.hh"

class PersonTraitPact : public TraitPact
{
public:
  PersonTraitPact(){}
  virtual ~PersonTraitPact(){}

  virtual void InitializeTraits();


  
  float Restlessness;
  float Gluttoness;
  float FoodBuyingThreshold;
  float Food2BuyAtOnce;
  float InvestmentLevel;
  
};


#endif /* __PERSONTRAITPACT_HH */
