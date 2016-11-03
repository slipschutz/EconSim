
#ifndef __PERSON_HH
#define __PERSON_HH

#include <sstream>

#include <map>
#include <vector>
using namespace std;


#include "PersonTraitPact.hh"
#include "EconomicActor.hh"
class Company;
class ActorLogger;

class Person : public EconomicActor {
public:
  Person(EconomicActorManager* ,bool DoInitialize=true);
  ~Person();
 
  void Initialize();
  void Initialize(Person *);

  void CopyTraits(const PersonTraitPact &);
  void CopyAndMutateTraits(const PersonTraitPact &);

  void DumpConnections();
  void DoStep();

  ActorActions EndOfStep();
  ActorActions BeginningOfStep();

  bool CheckTransactionMatch(int,EconomicActor*);  
  
  int GetActorType(){return ActorTypes::Person;}
  
  void DumpHavesWants();
  
  double GetWorth(Good);
  void YourFired(){rHaveAJob=false;rEmployerId=-1;rWasFiredInPreviousStep=true;rEmployer=NULL;}
  
  double GetCompanyInvestment();

  void GetPaid(double amount,string notes);

  void RemoveCompany(Company*);

  void PrintInfo();


  void SetRestlessnes(int v){rMyTraits.Restlessness=v;}
  void SetGluttoness(int v){rMyTraits.Gluttoness=v;}
  void SetInvestmentLevel(int v){rMyTraits.InvestmentLevel=v;}


  bool GetHasJob(){return rHaveAJob;}
  Company * GetEmployer(){return rEmployer;}
  
private:

  void rDoTransaction(int Good2Buy,int AmountOfGoodIWant,double price,int Seller);


  //Some person specific traits
  //all things 0-100
  // int rRestlessness;//How likely person will develop a want other than food
  // int rGluttoness;
  // int rFoodBuyingThreshold;
  // int rFood2BuyAtOnce;
  // int rInvestmentLevel;

  PersonTraitPact rMyTraits;


  
  bool rHaveAJob;
  
  int rEmployerId;
  Company * rEmployer;

  vector <Company*> rOwnedCompanies;

  double rPreviousSalary;
  
  bool rWasFiredInPreviousStep;

  bool rWasPaidInPreviousStep;
  stringstream rPaidNotes;
};
#endif
