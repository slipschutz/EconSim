
#ifndef __MANUFACTURERTRAITPACT_HH
#define __MANUFACTURERTRAITPACT_HH

#include "TraitPact.hh"
class ManufacturerTraitPact : public TraitPact
{

  
public:
  ManufacturerTraitPact();
  virtual ~ManufacturerTraitPact();
  virtual void InitializeTraits();
  void MutateCopy();

  double Conservativeness;//How likely company is too sell all of its supply
  double Steadfastness;//How likely company will lower it's price
  double PriceChangeLevel;//percent that change of price that is done when price is changed

  double StartingSalary;
  
};



#endif /* __MANUFACTURERTRAITPACT_HH */
