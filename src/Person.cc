

#include <Person.hh>


Person::Person() : rNumConnections(0){
  SetUniqueId(Person::GetNextId());
}

Person::~Person(){
  rConnections.clear();
}



void Person::Initialize(){
  std::cout<<"This is person Intializer"<<std::endl;
}

void Person::MakeConnection(Person * p){
  if (CheckConnection(p) && p->CheckConnection(this)){  
    rConnections[p->GetUniqueId()]=p;
    ((*p->GetConnections())[this->GetUniqueId()])=this;
    this->IncrementConnections();
    p->IncrementConnections();
  } 
}

bool Person::CheckConnection(Person* p){
  if (this->rConnections.count(p->GetUniqueId()) == 0){
    return true;
  } else {
    stringstream ss;
    ss<<"Attempted to add a duplicate connection between person "<<this->GetUniqueId()<<" and person "<<p->GetUniqueId();
    ErrorManager::BoxPrint(ss.str());
    return false;
  }

}

void Person::DumpConnections(){
  cout<<"Connections For Person "<<this->GetUniqueId()<<endl;
  for (map<int,Person*>::iterator ii=rConnections.begin();ii!=rConnections.end();
       ++ii){
    cout<<"Person "<<this->GetUniqueId()<<" connects to "<<ii->first<<" "<<ii->second->GetUniqueId()<<endl;

  }

}

