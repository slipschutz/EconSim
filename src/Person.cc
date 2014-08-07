#include "Person.hh"
#include "RandomManager.hh"
#include "PersonLogger.hh"

Person::Person() : rNumConnections(0){
  
}

Person::~Person(){
  rConnections.clear();
  rWants.clear();
  rHaves.clear();
  //  cout<<"Person Deconstructor"<<endl;
}



void Person::Initialize(){
  rMoney = RandomManager::GetRand(1000);
  
  int NumHaves=10;
  int NumWants=10;
  
  rHaves.clear();
  rWants.clear();
  for (int i=0;i<NumHaves;i++){
    int x = RandomManager::GetRand(100);
    Good aGood;
    //    aGood.SetPriority(RandomManager::GetRand(101));
    //    aGood.SetPriority(50);
    rHaves[x]=aGood;
  }
  for (int i=0;i<NumWants;i++){
    int x = RandomManager::GetRand(100);
    if (rHaves.count(x) ==0 ){ //if person does not already have this want
      Good aGood;
      //      aGood.SetPriority(RandomManager::GetRand(101));
      aGood.SetPriority(50);
      rWants[x]=aGood;
    } else {
      //Person already has this. So can't want it
      i--;
    }
  }

  
}

void Person::DumpHavesWants(){
  cout<<"Wants for Person "<<this->GetBaseId()<<endl;
  for (map<int,Good>::iterator ii=rWants.begin();ii!=rWants.end();ii++){
    cout<<"  "<<ii->first;
  }
  cout<<endl;
  cout<<"Haves for Person "<<this->GetBaseId()<<endl;
  for (map<int,Good>::iterator ii=rHaves.begin();ii!=rHaves.end();ii++){
    cout<<"  "<<ii->first;
  }
  cout<<endl;

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

void Person::MakeTransactions(PersonLogger *theLogger){
  //cout<<"In Make Transaction for "<<this->GetBaseId()<<endl;
  vector <int> GoodsToBeRemovedFromWants;
  for (map<int,Good>::iterator it_wants = rWants.begin();it_wants!=rWants.end();it_wants++){
    int thisWant= it_wants->first;
    for (map<int,Person*>::iterator ii= rConnections.begin();
	 ii!=rConnections.end();ii++){
      if (CheckTransactionMatch(thisWant,ii->second)){
	int num = RandomManager::GetRand(101);
	if (num >= it_wants->second.GetPriority()){
	  theLogger->CheckAndLogTransaction(this,ii->second,thisWant,num);

	  //cout<<"Make Transactiion person "<<this->GetBaseId()<<" wants "<<thisWant<<" person "<< ii->first<<" has it "<<endl;
	  //Make a transcation 
	  this->AddAGood(thisWant);
	  GoodsToBeRemovedFromWants.push_back(thisWant);
	  ii->second->RemoveAGood(thisWant);
	  break;//End loop over people looking for thiswant
	}
      }
    }
  }
  
  for (int i=0;i<GoodsToBeRemovedFromWants.size();i++){
    int GoodNumber=GoodsToBeRemovedFromWants[i];
  
    map<int,Good>::iterator ii =rWants.find(GoodNumber);
    if (ii == rWants.end()){
      //The Good being added is not in the list of wants
      //Something has gone wrong
      throw 1;
    } else {
      //cout<<"Erasing "<<ii->first<<" from rWants in person "<<this->GetBaseId()<<endl;
      rWants.erase(ii);
    }
  }
}
    
void Person::AddAGood(int GoodNumber){
  if (rHaves.count(GoodNumber)==0){
    rHaves[GoodNumber]=Good();
    //cout<<"Adding good "<<GoodNumber<<" to rHaves for person "<<this->GetBaseId()<<endl;
  }else {
    cout<<"Warning trying to add good "<<GoodNumber<<" when it is already in rHaves"<<endl;
  }
}

void Person::RemoveAGood(int GoodNumber){
  if (rHaves.count(GoodNumber)!=0){
    rHaves.erase(GoodNumber);
    //cout<<"Removing good from rHaves "<<GoodNumber<<" for person "<<this->GetBaseId()<<endl;
  } else {
    cout<<"Trying to remove "<<GoodNumber<<" but it is not in rHaves"<<endl;
  }
}

bool Person::CheckTransactionMatch(int num,Person* p){
  if ( p->rHaves.count(num) == 1){
    return true;
  } else {
    return false;
  }
}
