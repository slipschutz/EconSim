#include "Person.hh"
#include "RandomManager.hh"
#include "PersonLogger.hh"

int largestGoodNum=100;

Person::Person() : rNumConnections(0){
  
}

Person::~Person(){
  rConnections.clear();
  rWants.clear();
  rHaves.clear();
  //  cout<<"Person Deconstructor"<<endl;
}



void Person::Initialize(){
  //Randomly Pick starting money 
  //  rMoney = RandomManager::GetRand(1000);
  rMoney =1000;
  int NumHaves=10;
  int NumWants=10;
  
  rHaves.clear();
  rWants.clear();
  //Construct the initial haves for the person
  for (int i=0;i<NumHaves;i++){
    //Randomly Pick a good number
    int x = RandomManager::GetRand(largestGoodNum);
    Good aGood;
    aGood.SetGoodId(x);
    aGood.SetPriority(10);//Defult haves priority to 10 even though it is not used

    rHaves[x]=aGood;
  }

  //Construct the initial wants for the person
  for (int i=0;i<NumWants;i++){
    int x = RandomManager::GetRand(largestGoodNum);
    if (rHaves.count(x) ==0 ){ //if person does not already have this want
      Good aGood;
      aGood.SetPriority(RandomManager::GetRand(101));
      aGood.SetGoodId(x);
      rWants[x]=aGood;
    } else {
      //Person already has this. So can't want it
      i--;//Decrease loop index to keep going untill it finds enough goods
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
	double Worth2Buyer = GetWorth(it_wants->second);
	double Worth2Seller = GetWorth(ii->second->GetHaves()[thisWant]);
	if (Worth2Buyer > Worth2Seller){
	  //
	  // Log the transaction 
	  //
	  theLogger->CheckAndLogTransaction(this,ii->second,thisWant,Worth2Buyer,Worth2Seller);
	  
	  //
	  //Make the transcation 
	  //
	  this->AddAGood(thisWant);
	  GoodsToBeRemovedFromWants.push_back(thisWant);
	  ii->second->RemoveAGood(thisWant);
	  
	  double average = 0.5*(Worth2Buyer + Worth2Seller);
	  
	  ///Exchange the money
	  this->SubtractMoney(average);
	  ii->second->AddMoney(average);
	  theLogger->LogMoney(this,ii->second);
	  
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



void Person::EndOfStep(){
  //Preform End of step things

  
  //increment the wants priorities
  for (map<int,Good>::iterator ii = rWants.begin();ii!=rWants.end();ii++){
    ii->second.IncrementPriority();
  }
  
  ///Randomly add wants 
  
  if (RandomManager::GetRand(101)>90){
    int newWant= RandomManager::GetRand(largestGoodNum);
    if (rWants.count(newWant)==0){
      Good aWant;
      aWant.SetGoodId(newWant);
      aWant.SetPriority(RandomManager::GetRand(101));
      rWants[newWant]=aWant;
    }
  }

  
}

double Person::GetWorth(Good theGood){
  double Priority2Money =1.5;
  double WantNumber2Money=1.5;

  int GoodNumber = theGood.GetGoodId();
  int Priority =theGood.GetPriority();

  int NumberOfPeopleThatWantTheGood=0;
  int NumberOfPeopleThatHaveTheGood=0;
  
  for (map<int,Person*>::iterator ii = rConnections.begin();
       ii!=rConnections.end();ii++){
    if (ii->second->GetWants().count(GoodNumber) !=0){
      NumberOfPeopleThatWantTheGood++;
    }
    if (ii->second->GetHaves().count(GoodNumber) !=0){
      NumberOfPeopleThatHaveTheGood++;
    } 
  }


  return Priority2Money*Priority + WantNumber2Money*NumberOfPeopleThatWantTheGood;
	   

}
