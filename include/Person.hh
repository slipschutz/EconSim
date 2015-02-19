
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
  void YourFired(){rHaveAJob=false;}
  
private:

  //Some person specific traits
  //all things 0-100
  int rRestlessness;//How likely person will develop a want other than food
  int rGluttoness;//How much food some one eats
  
  bool rHaveAJob;
  

};
#endif
