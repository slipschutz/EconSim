
#ifndef __COMPANY_HH
#define __COMPANY_HH


//include standard headers
#include <vector>
#include <iostream>
#include <string>
using namespace std;


//Include local headers
#include "Person.hh"

/**ABC for the company chain of classe.  Will be base class
   for the different types of companies.  Manufacturer, retailer ect...
*/

class Company : public EconomicActor{

public:
  Company(EconomicActorManager * man,Person *own);
  Company(EconomicActorManager * man) : EconomicActor(man){cout<<"Company constrcut no owner never call"<<endl; throw 1;}
  virtual ~Company();
  
  void SetNoOwner(){fTheOwner=NULL;}

  Person* GetTheOwner(){return fTheOwner;}
  /**  */
  // virutal void Initialize();
  // void EndOfStep(){}
  // void BeginningOfStep();
  // void DoStep(){}
  // bool CheckTransactionMatch(int, EconomicActor*){return true;}
  // virtual int GetActorType(){return ActorTypes::Company;}
  void AddEmployee(Person * ,double salary);
  void RemoveEmployee(Person *);

  int GetNumberOfEmployees(){return Employees2Salary.size();}
protected:
 
  map<Person*,double> Employees2Salary;

  double fCurrentPersonnelCost;


  Person* fTheOwner;
  int fTheOwnerId;
  
};



#endif /* __COMPANY_HH */
