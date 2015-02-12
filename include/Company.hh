
#ifndef __COMPANY_HH
#define __COMPANY_HH


//include standard headers
#include <vector>
#include <iostream>
#include <string>
using namespace std;


//Include local headers
#include "EconomicActor.hh"


class Company : public EconomicActor{

public:
  Company();
  virtual ~Company();

  
  /**  required class from EconomicActor*/
  void Initialize();
  void EndOfStep(){}
  void BeginningOfStep(){}
  void DoStep(){}
  bool CheckTransactionMatch(int, EconomicActor*){return true;}
  int GetActorType(){return ActorTypes::Company;}
 
  
};



#endif /* __COMPANY_HH */
