





#ifndef __MANUFACTURER_HH
#define __MANUFACTURER_HH

#include <vector>
#include <iostream>
#include <string>



//Local Headers 
#include "Company.hh"
#include "ManufacturerTraitPact.hh"

using namespace std;

class Manufacturer : public Company {
public:
  Manufacturer(double money,EconomicActorManager*man,Person * own, int GoodNumber,bool DoInitialize=true) :
    Company(man,own),GoodToManufacture(GoodNumber) {
    fMoney=money;
    if (DoInitialize){
      Initialize();
    }
  }

  virtual ~Manufacturer(){
    
  }

  
  /**Required Abstract methods */
  void Initialize();
  ActorActions EndOfStep();
  ActorActions BeginningOfStep();
  void DoStep();
  bool CheckTransactionMatch(int, EconomicActor*){return true;}


  //Return the type ID for this type of actor
  virtual int GetActorType(){return ActorTypes::Manufacturer;}


  void DumpConnections();
  int GetGoodToManufacture(){return GoodToManufacture;}

  void PrintInfo();
private:

  int GoodToManufacture;

  double rConservativeness;//How likely company is too sell all of its supply
  double rSteadfastness;//How likely company will lower it's price
  double rPriceChangeLevel;//percent that change of price that is done when price is changed 

  float rSalaryChangeLikelyhood;
  
  int MaxVolume;//Max amount of good that company can make
  
  float rStartingSalary;
  
  int rTotalVolumeCreated;

  double rStartOfStepMoney;
  int rStartOfStepSupply;

  int numberOfSteps;
  int numberOFProductions;

  double rGoodPrice;
  
  bool rFirstStep;
  bool rHireMorePeople;

  
  ManufacturerTraitPact myTraits;
  
};



#endif /* __MANUFACTURER_HH */
