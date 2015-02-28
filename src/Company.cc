

#include "Company.hh"
#include "Settings.hh"

#include "RandomManager.hh"

Company::Company(EconomicActorManager* man,Person* own ) : EconomicActor(man), fTheOwner(own){

  Employees2Salary.clear();


}


Company::~Company(){
  //This compnay is being deleted.  Clean up its employees
  for (auto i : Employees2Salary){
    i.first->YourFired();
  }
  Employees2Salary.clear();//Clear list of pointers
  

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
