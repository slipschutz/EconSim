#ifndef __MANUFACTURERTRAITPACT_HH
#define __MANUFACTURERTRAITPACT_HH

#include "TraitPact.hh"
class ManufacturerTraitPact : public TraitPact
{
public:
  static float TraitMutationRate;
  
  ManufacturerTraitPact();
  virtual ~ManufacturerTraitPact();
  virtual void InitializeTraits();
  void MutateCopy(const ManufacturerTraitPact &);

  float Conservativeness;//How likely company is too sell all of its supply
  float Steadfastness;//How likely company will lower it's price
  float PriceChangeLevel;//percent that change of price that is done when price is changed
  float StartingSalary;

  

  
};



#endif /* __MANUFACTURERTRAITPACT_HH */
