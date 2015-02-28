






#ifndef __DEADACTOR_HH
#define __DEADACTOR_HH

#include <vector>
#include <iostream>
#include <string>

#include "EconomicActor.hh"
using namespace std;

class DeadActor : public EconomicActor{
public:
  DeadActor(EconomicActorManager * man) : EconomicActor(man){
  }
  virtual ~DeadActor(){
    //    cout<<"in ~Dead Actor "<<this<<endl;
  }

  void Initialize(){}
  
  ActorActions BeginningOfStep(){return ActorActions::None;}
  ActorActions EndOfStep(){return ActorActions::None;}//Already dead can't die again
  void Intialize(){}
  void DoStep(){}
  bool CheckTransactionMatch(int,EconomicActor*){return false;}
  int GetActorType(){return -1;}

};



#endif /* __DEADACTOR_HH */
