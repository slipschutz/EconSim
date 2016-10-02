


#ifndef __RETAILER_HH
#define __RETAILER_HH

#include <vector>
#include <iostream>
#include <string>

#include "Company.hh"
#include "Settings.hh"
#include "RandomManager.hh"
using namespace std;

class Retailer : public Company 
{
public:
  Retailer(EconomicActorManager *man):Company(man){}
  virtual ~Retailer(){}
  
  void Initialize(){
    cout<<"The good priorities has size "<<fGoodPriorities.size()<<endl;
    cout<<"Max Good number "<<Settings::MaxGoodNumber<<endl;
    for (unsigned int i=0;i<fGoodPriorities.size();i++){
      fGoodPriorities[i]=RandomManager::GetRand(Settings::MaxGoodPriority);
    }
  }

  ActorActions EndOfStep(){return ActorActions::None;}
  ActorActions BeginningOfStep(){return ActorActions::None;};
  void DoStep(){}
  bool CheckTransactionMatch(int, EconomicActor*){return true;}
  int GetActorType(){return 111;}

};



#endif /* __RETAILER_HH */
