#ifndef __PERSONLOGGER_HH
#define __PERSONLOGGER_HH

#include "Base.hh"
#include <vector>
class PersonSnapshot;
class Person;

class PersonLogger : public Base{

public:
  PersonLogger();
  ~PersonLogger();
  
  void Initialize();
  
  void CheckAndLogTransaction(Person*,Person*,int GoodBeingTraded,double value2Buyer,double value2Seller);
  
  void LogMoney(Person* p1,Person* p2);

  void DumpLog();
  
  void SetPerson(int v){rThePerson=v;}
private:
  
  int rThePerson;
  
  std::vector <PersonSnapshot> rWantsSnapshots;
  std::vector <PersonSnapshot> rHasSnapshots;
  std::vector <int> rGoodBeingTraded;
  std::vector <double> rValue2Buyer;
  std::vector <double> rValue2Seller;

  std::vector <double> rMoneyLog;

};

#endif
