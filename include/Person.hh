
#ifndef __PERSON_HH
#define __PERSON_HH

#include <sstream>

#include <map>
#include <vector>
using namespace std;


#include "EconomicActor.hh"
class Company;
class ActorLogger;

class Person : public EconomicActor {
public:
  Person(EconomicActorManager* );
  ~Person();
 
  void Initialize();
  void Initialize(Person *);


  void DumpConnections();
  void DoStep();

  ActorActions EndOfStep();
  ActorActions BeginningOfStep();

  bool CheckTransactionMatch(int,EconomicActor*);  
  
  int GetActorType(){return ActorTypes::Person;}
  
  void DumpHavesWants();
  
  double GetWorth(Good);
  void YourFired(){rHaveAJob=false;rEmployerId=-1;rWasFiredInPreviousStep=true;rEmployer=NULL;}
  
  double GetCompanyInvestment(){return (rInvestmentLevel/100.)*fMoney;}

  void GetPaid(double amount,string notes);

  void RemoveCompany(Company*);

  void PrintInfo();


  void SetRestlessnes(int v){rRestlessness=v;}
  void SetGluttoness(int v){rGluttoness=v;}
  void SetInvestmentLevel(int v){rInvestmentLevel=v;}


private:

  void rDoTransaction(int Good2Buy,int AmountOfGoodIWant,double price,int Seller);
  //Some person specific traits
  //all things 0-100
  int rRestlessness;//How likely person will develop a want other than food
  int rGluttoness;//How much food some one eats


  int rInvestmentLevel;
  
  bool rHaveAJob;
  
  int rEmployerId;
  Company * rEmployer;

  vector <Company*> rOwnedCompanies;


  
  bool rWasFiredInPreviousStep;

  bool rWasPaidInPreviousStep;
  stringstream rPaidNotes;
};
#endif
