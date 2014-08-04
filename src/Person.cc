#include "Person.hh"


Person::Person() : rNumConnections(0){
  //  SetUniqueId(Person::GetNextId());
  // cout<<"Person Constructor"<<endl;
}

Person::~Person(){
  rConnections.clear();
  //  cout<<"Person Deconstructor"<<endl;
}



void Person::Initialize(){
  std::cout<<"This is person Intializer"<<std::endl;
}

void Person::MakeConnection(Person * p){
  if (CheckConnection(p) && p->CheckConnection(this)){  
    rConnections[p->GetBaseId()]=p;
    (*(p->GetConnections()))[this->GetBaseId()]=this;
    this->IncrementConnections();
    p->IncrementConnections();
  } 
}

bool Person::CheckConnection(Person* p){
  if (this->rConnections.count(p->GetBaseId()) == 0){
    return true;
  } else {
    //   stringstream ss;
    //ss<<"Attempted to add a duplicate connection between person "<<this->GetBaseId()<<" and person "<<p->GetBaseId();
    //ErrorManager::BoxPrint(ss.str());
    return false;
  }

}

void Person::DumpConnections(){
  cout<<"Connections For Person "<<this->GetBaseId()<<endl;
  for (map<int,Person*>::iterator ii=rConnections.begin();ii!=rConnections.end();
       ++ii){
    cout<<"    Person "<<this->GetBaseId()<<" connects to "<<ii->first<<endl;

  }

}

