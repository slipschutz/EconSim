
#ifndef __PERSONWITHNEURAL_HH
#define __PERSONWITHNEURAL_HH

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include "EconomicActor.hh"
#include "ActorTypes.hh"

#include "NeuralNetworkInterface.hh"

class EconomicActorManager;


using namespace std;

class PersonWithNeural : public EconomicActor
{
public:

  PersonWithNeural(EconomicActorManager*);
  virtual ~PersonWithNeural();

  void Initialize(){}
  bool CheckTransactionMatch(int, EconomicActor*){return false;}
  
  ActorActions BeginningOfStep();
  ActorActions EndOfStep();
  void DoStep();

  int GetActorType(){return ActorTypes::PersonWithNeural;}

  void rDoTransaction(int Good2Buy,int AmountOfGoodIWant,double price,int Seller);
  
private:
  NeuralNetworkInterface * rBrain;
  stringstream rMessage;
};



#endif /* __PERSONWITHNEURAL_HH */
