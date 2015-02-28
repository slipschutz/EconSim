
#ifndef __COMPANY_HH
#define __COMPANY_HH


//include standard headers
#include <vector>
#include <iostream>
#include <string>
using namespace std;


//Include local headers
#include "EconomicActor.hh"

class Person;
/**ABC for the company chain of classe.  Will be base class
   for the different types of companies.  Manufacturer, retailer ect...

 */

class Company : public EconomicActor{

public:
  Company();
  virtual ~Company();

  
  /**  */
  // virutal void Initialize();
  // void EndOfStep(){}
  // void BeginningOfStep();
  // void DoStep(){}
  // bool CheckTransactionMatch(int, EconomicActor*){return true;}
  // virtual int GetActorType(){return ActorTypes::Company;}
  void AddEmployee(Person * ,double salary);
  void RemoveEmployee(Person *);
protected:
 
  map<Person*,double> Employees2Salary;
  
  
};



#endif /* __COMPANY_HH */
