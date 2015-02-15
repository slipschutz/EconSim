
#ifndef __PERSON_HH
#define __PERSON_HH

#include <sstream>

#include <map>
#include <vector>
using namespace std;


#include "EconomicActor.hh"


class Person : public EconomicActor {
public:
  Person();
  ~Person();
 
  void Initialize();


  void DumpConnections();
  void DoStep();

  bool EndOfStep();
  void BeginningOfStep();

  bool CheckTransactionMatch(int,EconomicActor*);  
  
  int GetActorType(){return ActorTypes::Person;}
  
  void DumpHavesWants();
  
  double GetWorth(Good);
  

private:

  

};
#endif
