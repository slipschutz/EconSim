

#include "Company.hh"
#include "Settings.hh"

#include "RandomManager.hh"

Company::Company(){

  Employees2Salary.clear();


}


Company::~Company(){
  //  cout<<"~company()"<<endl;


}

void Company::AddEmployee(Person *a ,double salary){
  
  auto it =Employees2Salary.find(a);

  if(it == Employees2Salary.end()){//Employee isn't there
    Employees2Salary.insert(make_pair(a,salary));
  }
}
