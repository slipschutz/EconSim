#include "Person.hh"
#include "RandomManager.hh"
#include "PersonLogger.hh"
#include "GoodManager.hh"
#include "DataLogger.hh"
int largestGoodNum=1;

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
  rMoney =100000;
  int NumHaves=1;
  int NumWants=1;

  
  rHaves.clear();
  rWants.clear();

  // if(this->GetBaseId() == 6){ //Guy with all the stuff
  //   rHaves[0]=GoodManager::Get()->MakeHave(RandomManager::GetRand(2),10);
  // } else {
  //   rWants[0]=GoodManager::Get()->MakeWant(0,400);
  // }

  rHaves[0]=GoodManager::Get()->MakeHave(0,RandomManager::GetRand(8)+1);
  rWants[0]=GoodManager::Get()->MakeWant(0,RandomManager::GetRand(800)+1);

}

void Person::DumpHavesWants(){
  cout<<"Wants for Person "<<this->GetBaseId()<<":   ";
  for (map<int,Good>::iterator ii=rWants.begin();ii!=rWants.end();ii++){
    cout<<"  "<<ii->first<<"("<<ii->second.GetNumberOfCopies()<<")["<<ii->second.GetPriority()<<"]";
  }
  cout<<endl;
  cout<<"Haves for Person "<<this->GetBaseId()<<":   ";
  for (map<int,Good>::iterator ii=rHaves.begin();ii!=rHaves.end();ii++){
    cout<<"  "<<ii->first<<"("<<ii->second.GetNumberOfCopies()<<")["<<ii->second.GetPriority()<<"]";
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
	cout<<" Worth  "<<Worth2Buyer<<"  "<<Worth2Seller<<endl;
	if (Worth2Buyer >= Worth2Seller && Worth2Seller <=rMoney ){
	  //
	  // Log the transaction 
	  //
	  theLogger->CheckAndLogTransaction(this,ii->second,thisWant,Worth2Buyer,Worth2Seller);
	  
	  //
	  //Make the transcation 
	  //
	  this->AddAGood(thisWant);//Add to haves
	  GoodsToBeRemovedFromWants.push_back(thisWant);//Will remove from wants later

	  ii->second->RemoveAGood(thisWant);//Remove from Haves of other person
	  
	  //	  double average = 0.5*(Worth2Buyer + Worth2Seller);
	  
	  ///Exchange the money
	  this->SubtractMoney(Worth2Seller);
	  ii->second->AddMoney(Worth2Seller);
	  theLogger->LogMoney(this,ii->second);
	  
	  DataLogger::Get()->PushGoodPrice(thisWant,Worth2Seller);

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
      //Removing a want from the world
      //      rWants.erase(ii);
      int t=ii->second.GetNumberOfCopies();
      if (t ==1){ //Delete from list completely
	rWants.erase(ii);
      }else{
	ii->second.SetNumberOfCopies(t-1);
      }
      //now remove the want from the global demand stored in the goodmanager
      
      (GoodManager::Get()->demand)[GoodNumber]--;
    }
  }
}
    

void Person::MakeTransactions(){
  ///Loop over all connections and see if any of them have 
  ///things to buy or sell

  vector <int> GoodsToBeRemovedFromWants;

  for (auto & connection : rConnections){
    for (auto & thisWant : rWants ){
      //Does this connection have this want
      if (connection.second->GetHaves().count(thisWant.first) !=0){
	//the connection has this want.  This person wants to buy it
	double Worth2Buyer=GoodManager::Get()->GetWorthToBuyer(this,thisWant.first);
	double Worth2Seller=GoodManager::Get()->GetWorthToSeller(connection.second,thisWant.first);


	if(Worth2Buyer >= Worth2Seller && Worth2Seller <= this->GetMoney()){
	  //Do the transaction where this person is buying the good from connection
	  
	  this->AddAGood(thisWant.first);//Add to haves
	  connection.second->RemoveAGood(thisWant.first);//Remove from Haves of other person

	  GoodsToBeRemovedFromWants.push_back(thisWant.first);//Will remove from wants later
	  //can't remove things from map while iterating over it.
	  
	  ///Exchange the money
	  this->SubtractMoney(Worth2Seller);
	  connection.second->AddMoney(Worth2Seller);
	  
	  
	  DataLogger::Get()->PushGoodPrice(thisWant.first,Worth2Seller);

	  

	}//End if Worth2Buyer>=Worth2Seller
      }//End if connection has this good
    }//End loop over Wants
 
    //Clean up from this person buying a good.  Want for this p person needs to be removed

    for (int i=0;i<GoodsToBeRemovedFromWants.size();i++){
      int GoodNumber=GoodsToBeRemovedFromWants[i];
  
      map<int,Good>::iterator ii =rWants.find(GoodNumber);
      if (ii == rWants.end()){
	//The Good being added is not in the list of wants
	//Something has gone wrong
	throw 1;
      } else {
	//Removing a want from the world
	//      rWants.erase(ii);
	int t=ii->second.GetNumberOfCopies();
	if (t ==1){ //Delete from list completely
	  rWants.erase(ii);
	}else{
	  ii->second.SetNumberOfCopies(t-1);
	}
	//now remove the want from the global demand stored in the goodmanager
	(GoodManager::Get()->demand)[GoodNumber]--;
      }
    }
    GoodsToBeRemovedFromWants.clear();

    for (auto & thisHave : rHaves){
      //does this connection want this have
      if (connection.second->GetWants().count(thisHave.first)!=0){
	//the connection wants this have.  This person wants to sell it
	double Worth2Seller=GoodManager::Get()->GetWorthToSeller(this,thisHave.first);
	double Worth2Buyer=GoodManager::Get()->GetWorthToBuyer(connection.second,thisHave.first);
	if(Worth2Buyer >= Worth2Seller && Worth2Seller <= connection.second->GetMoney()){
	  //Do the transaction
	}
      }
    }
  }
}


void Person::AddAGood(int GoodNumber){
  if (rHaves.count(GoodNumber)==0){
    rHaves[GoodNumber]=GoodManager::Get()->ReplaceHave(GoodNumber,1);
    //cout<<"Adding good "<<GoodNumber<<" to rHaves for person "<<this->GetBaseId()<<endl;
  }else {
    //The good is already there person now has a second copy of the good
    int t=rHaves[GoodNumber].GetNumberOfCopies() + 1;
    rHaves[GoodNumber].SetNumberOfCopies(t);
  }
}

void Person::RemoveAGood(int GoodNumber){
  if (rHaves.count(GoodNumber)!=0){
    int t = rHaves[GoodNumber].GetNumberOfCopies();
    if ( t ==1){
      rHaves.erase(GoodNumber);
    } else {
      rHaves[GoodNumber].SetNumberOfCopies(t-1);
    }
  } else {
    ///The good isn't there.  Silently do nothing
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
  double Priority2Money =0;
  double WantNumber2Money=1.5;

  int GoodNumber = theGood.GetGoodId();
  int Priority =theGood.GetPriority();

  int NumberOfPeopleThatWantTheGood=GoodManager::Get()->demand[GoodNumber];
  int NumberOfPeopleThatHaveTheGood=0;
  


  return Priority2Money*Priority + WantNumber2Money*NumberOfPeopleThatWantTheGood;
	   

}
