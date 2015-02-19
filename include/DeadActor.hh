






#ifndef __DEADACTOR_HH
#define __DEADACTOR_HH

#include <vector>
#include <iostream>
#include <string>

#include "EconomicActor.hh"
using namespace std;

class DeadActor : public EconomicActor{
public:
  DeadActor(){
  }
  virtual ~DeadActor(){
    //    cout<<"in ~Dead Actor "<<this<<endl;
  }

  void Initialize(){}
  
  void BeginningOfStep(){}
  bool EndOfStep(){return false;}//Already dead can't die again
  void Intialize(){}
  void DoStep(){}
  bool CheckTransactionMatch(int,EconomicActor*){return false;}
  int GetActorType(){return -1;}

};



#endif /* __DEADACTOR_HH */
