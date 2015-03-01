

#include "Company.hh"
#include "Settings.hh"

#include "RandomManager.hh"
#include "Death.hh"

Company::Company(EconomicActorManager* man,Person* own ) : EconomicActor(man), fTheOwner(own){

  Employees2Salary.clear();
  fTheOwnerId=fTheOwner->GetBaseId();

}


Company::~Company(){
  //This compnay is being deleted.  Clean up its employees

  for (auto i : Employees2Salary){
    i.first->YourFired();
  }
  Employees2Salary.clear();//Clear list of pointers

  //Also inform the owner that this company is nolonger in busines

  if (! Death::Get()->CheckDeath(fTheOwnerId)){
    fTheOwner->RemoveCompany(this);
  }

}

void Company::AddEmployee(Person *a ,double salary){
  
  auto it =Employees2Salary.find(a);

  if(it == Employees2Salary.end()){//Employee isn't there
    Employees2Salary.insert(make_pair(a,salary));
    fCurrentPersonnelCost+=salary;
  }
}


void Company::RemoveEmployee(Person * p){
  auto it = Employees2Salary.find(p);
  
  if (it != Employees2Salary.end()){//Employees is there
    fCurrentPersonnelCost-=it->second;
    Employees2Salary.erase(it);
  }

}
