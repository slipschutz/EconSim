#ifndef __ACTORLOGGER_HH
#define __ACTORLOGGER_HH

#include "Base.hh"
#include <vector>
#include <fstream>

class EconomicActor;
#include "ActorSnapshot.hh"
class ActorLogger : public Base{

public:

  static ActorLogger* theLogger;
  static ActorLogger* Get();

  ~ActorLogger(){}
  
  void Initialize(){}
  

  void LogBeforeStepState(EconomicActor*);
  void LogAfterStepState(EconomicActor*);

  void BeforeMessage(string s){rBeforeMessage.push_back(s);}
  void DuringMessage(string s){rDuringMessage.push_back(s);}
  void EndMessage(string s){rAfterMessage.push_back(s);}


  int thePerson;

  void DumpLog();
  

private:
  
  ActorLogger(){}
  
  // std::vector <ActorSnapshot> rWantsSnapshots;
  // std::vector <ActorSnapshot> rHasSnapshots;
  // std::vector <int> rGoodBeingTraded;
  // std::vector <double> rValue2Buyer;
  // std::vector <double> rValue2Seller;

  std::vector <double> rMoneyLog;

  std::vector <string> rBeforeState;
  std::vector <string> rAfterState;

  std::vector <string> rBeforeMessage;
  std::vector <string> rDuringMessage;
  std::vector <string> rAfterMessage;
  
  ofstream outFile;
};

#endif
