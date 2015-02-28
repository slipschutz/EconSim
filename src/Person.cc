#include "Person.hh"
#include "RandomManager.hh"
#include "ActorLogger.hh"
#include "GoodManager.hh"
#include "DataLogger.hh"
#include "MarketManager.hh"
#include "Calendar.hh"

#include "Company.hh"
#include "EconomicActorManager.hh"

int largestGoodNum=1;

Person::Person(EconomicActorManager* man) : EconomicActor(man){
  
}

Person::~Person(){
  //Need to clean up this economic actor 
  //

}



void Person::Initialize(){
  //Randomly Pick starting money 
  //  rMoney = RandomManager::GetRand(1000);
  fMoney =1000;

  fSupplies.clear();
  fDemands.clear();

  //Good ID 0 is food and should have the highest prioriry 
  fGoodPriorities[0]=Settings::MaxGoodPriority;


  AddSupply(0,100);
  
  //Initialize Person specific Traits 
  
  rRestlessness=RandomManager::GetRand(100);
  rGluttoness=RandomManager::GetRand(10)+1;
  rInvestmentLevel=RandomManager::GetRand(100);

  rHaveAJob=false;
  rEmployerId=-1;
  rWasFiredInPreviousStep=false;
}

void Person::DumpHavesWants(){
  cout<<"Wants for Person "<<this->GetBaseId()<<":   ";
  for (map<int,Good>::iterator ii=fDemands.begin();ii!=fDemands.end();ii++){
    cout<<"  "<<ii->first<<"("<<ii->second.GetNumberOfCopies()<<")["<<ii->second.GetPriority()<<"]";
  }
  cout<<endl;
  cout<<"Haves for Person "<<this->GetBaseId()<<":   ";
  for (map<int,Good>::iterator ii=fSupplies.begin();ii!=fSupplies.end();ii++){
    cout<<"  "<<ii->first<<"("<<ii->second.GetNumberOfCopies()<<")["<<ii->second.GetPriority()<<"]";
  }
  cout<<endl;

}



void Person::DumpConnections(){
  cout<<"Connections For Person "<<this->GetBaseId()<<endl;
  for (auto & ii : fConnections){
    cout<<"    Person "<<this->GetBaseId()<<" connects to "<<ii.first<<endl;
  }


}


ActorActions Person::BeginningOfStep(){

  ActorActions ret=ActorActions::None;

  //Check to see if this person has food (GOOD ID=0)
  if (fSupplies.count(0) == 0) {
    //Food is not in the supplies map
    //Must get food
    AddSupply(0,0);//Make empty good
  }

  stringstream s;
  s<<"Dear Diary, \n Today is day "<<Calendar::DayNumber<<endl;

  if (fSupplies[0].GetNumberOfCopies() < rGluttoness){
    //There are no copies of food in the supply
    int n=RandomManager::GetRand(10)+rGluttoness;
    AddDemand(0,n);//Add the demand for food

    s<<"I need Food.  I want to buy "<<n<<" foods"<<endl;
  }
  
  if (RandomManager::GetRand(100) < rRestlessness){
    int d=RandomManager::GetRand(Settings::MaxGoodNumber);
    AddDemand(d,1);
    s<<"Diary I just have to have a new "<<d<<" or else no one will like me"<<endl;
  }
  

  if (RandomManager::GetRand(100)< 2){
    s<<"Diary it is time i started a compnay"<<endl;
    ret=ActorActions::StartedCompany;
  }
  
  if (rWasFiredInPreviousStep==true){
    rWasFiredInPreviousStep=false;
    s<<"Diary i was fired in yesterday"<<endl;
  }
  
  if (this->GetBaseId() == ActorLogger::Get()->thePerson){
    ActorLogger::Get()->LogBeforeStepState(this);
    ActorLogger::Get()->BeforeMessage(s.str());
  }

  return ret;
}

void Person::DoStep(){
  //check priorties list for things to buy
  stringstream dayNotes;
  if (fDemandPriorities2GoodNum.size()==0){
    //Person doesn't want anything done with step
    dayNotes<<"All of my wants are fulfilled.  Today I will do nothing"<<endl;
  } else {
  
    //Buy the thing that has the most priority 
    auto it= fDemandPriorities2GoodNum.end();
    //move iterator 1 back from the end to get last elment
    it--;
    int Good2Buy = it->second;//this is the good number that has the highest priority
    int totalWant= fDemands[Good2Buy].GetNumberOfCopies();
    int AmountOfGoodIWant =totalWant;

    //  cout<<"This is person "<<this->GetBaseId()<<" I want "<<AmountOfGoodIWant<<endl;
    while (AmountOfGoodIWant >0){
      OrderInfo info;
      int Seller=MarketManager::Get()->GetCheapestSeller(Good2Buy,info);
      if (Seller == -1 ){// there is no seller 
	// cout<<"NO SELLER FOR "<<this->GetBaseId()<<endl;
	dayNotes<<"I wanted to buy "<<totalWant<<" of good "<<Good2Buy<<" but can't find seller "<<endl
		<<" at the end of the day I still want "<<AmountOfGoodIWant<<endl;
	break;
      } else if (info.Price*AmountOfGoodIWant > fMoney){
	//This price is always the current cheapest price
	//if I don't have enought money to buy what I want then 
	//Not sale
	dayNotes<<"I wanted to buy "<<info.Price*AmountOfGoodIWant<<" worth of good "<<Good2Buy<<endl
		<<"but I only have "<<fMoney<<endl;
	if (AmountOfGoodIWant > 5)
	  AmountOfGoodIWant-=5;
	else
	  break;

      } else{
	if (AmountOfGoodIWant <= info.Quantity ){
	  //All of the remaining good can be bought from this seller
	  double totalCost=(AmountOfGoodIWant)*info.Price;
	  //Move the money
	  fConnections[Seller]->AddMoney(totalCost);
	  this->SubtractMoney(totalCost);
      
	  //Remove the Supply from seller

	  GoodManager::Get()->ReconcileTransaction(fConnections[Seller],this,Good2Buy,AmountOfGoodIWant);
	  MarketManager::Get()->CleanUpOrder(Good2Buy,info.Price,Seller,AmountOfGoodIWant);
	  dayNotes<<"Haza I bought "<<AmountOfGoodIWant<<" of good "<<Good2Buy<<" from "<<Seller<<" at a price of "<<endl
		  <<info.Price<<" totaling "<<totalCost<<endl;
	  AmountOfGoodIWant=0;
	} else {
      
	  int PartOfWhatIWant=info.Quantity; // Buying everything this seller can offer

	  //This seller can only provide part of the order
	  double totalCost=(PartOfWhatIWant)*info.Price;
	  //Move the money
	  fConnections[Seller]->AddMoney(totalCost);
	  this->SubtractMoney(totalCost);
      
	  //Remove the Supply from seller
	  GoodManager::Get()->ReconcileTransaction(fConnections[Seller],this,Good2Buy,PartOfWhatIWant);
	  MarketManager::Get()->CleanUpOrder(Good2Buy,info.Price,Seller,PartOfWhatIWant);
	  AmountOfGoodIWant-=PartOfWhatIWant;
	  dayNotes<<"Less Haza I bought "<<info.Quantity<<" of good "<<Good2Buy<<" from "<<Seller<<" at a price of "<<endl
		  <<info.Price<<" totaling "<<totalCost<<endl;
	}
      }
    }
  }

  //Check to see if this person is employed
  if (!rHaveAJob){
    //Get a job
    JobInfo jInfo= MarketManager::Get()->GetBestJob();
    if (jInfo.EmployerID==-1){
      cout<<"No jobs "<<endl;
    }else{
      rHaveAJob=true;
      MarketManager::Get()->BrokerJob(this,(Company*)fConnections[jInfo.EmployerID],jInfo.salary);
      rEmployer=(Company*)fConnections[jInfo.EmployerID];
      rEmployerId=jInfo.EmployerID;
      dayNotes<<"I Got a job working for "<<jInfo.EmployerID<<" at salary "<<jInfo.salary<<endl;
    }
  }


  if (this->GetBaseId() == ActorLogger::Get()->thePerson){
    ActorLogger::Get()->DuringMessage(dayNotes.str());
  }

    
}

ActorActions Person::EndOfStep(){
  //cout<<"IN end of step for "<<this->GetBaseId()<<" SUpply "<<fSupplies[0].GetNumberOfCopies()<<endl;
  stringstream s;
  //Preform End of step things
  ActorActions ret;
  if (fSupplies[0].GetNumberOfCopies()< rGluttoness){
    //cout<<"\n\n\nI DIED"<<endl;
    s<<"I have died :( "<<endl;
    if (rEmployer!=NULL){
       rEmployer->RemoveEmployee(this);
    }
    
    ret=ActorActions::Died;
  }else {
    fSupplies[0].RemoveCopies(rGluttoness);
    //    GoodManager::Get()->RemoveSupply(0,1);
    s<<"I ate "<<rGluttoness<<" foods "<<endl;
    ret=ActorActions::None;
  }
  if (this->GetBaseId() == ActorLogger::Get()->thePerson){
    ActorLogger::Get()->LogAfterStepState(this);
    ActorLogger::Get()->EndMessage(s.str());
  }
  return ret;
}


void Person::PrintInfo(){
  PrintLine('v',30);
  cout<<"Info for <Person> Base id "<<this->GetBaseId()<<" actor type "<<this->GetActorType()<<endl;
  cout<<"Has Job? "<<rHaveAJob<<" employer "<<rEmployerId<<endl;
  cout<<"I have "<<fMoney<<" monies"<<endl;
  //  DumpSupplies();
  PrintLine('^',30);
}
 
/*
void DoBuy(int good,int connection){
  this->AddAGood(good);
  fConnections[connection]->RemoveAGood(good);
  this->SubtractMoney(

}


	  this->AddAGood(thisDemand.first);//Add to haves
	  connection.second->RemoveAGood(thisDemand.first);//Remove from Haves of other person

	  GoodsToBeRemovedFromWants.push_back(thisDemand.first);//Will remove from wants later
	  //can't remove things from map while iterating over it.
	  
	  ///Exchange the money
	  this->SubtractMoney(Worth2Seller);
	  connection.second->AddMoney(Worth2Seller);
	  
	  DataLogger::Get()->PushGoodPrice(thisDemand.first,Worth2Seller);




    //Clean up from this person buying a good.  Want for this p person needs to be removed

    for (int i=0;i<GoodsToBeRemovedFromWants.size();i++){
      int GoodNumber=GoodsToBeRemovedFromWants[i];
  
      map<int,Good>::iterator ii =fDemands.find(GoodNumber);
      if (ii == fDemands.end()){
	//The Good being added is not in the list of wants
	//Something has gone wrong
	throw 1;
      } else {
	//Removing a want from the world
	//      fDemands.erase(ii);
	int t=ii->second.GetNumberOfCopies();
	if (t ==1){ //Delete from list completely
	  fDemands.erase(ii);
	}else{
	  ii->second.SetNumberOfCopies(t-1);
	}
	//now remove the want from the global demand stored in the goodmanager
	(GoodManager::Get()->demand)[GoodNumber]--;
      }
    }
    GoodsToBeRemovedFromWants.clear();

*/




bool Person::CheckTransactionMatch(int num,EconomicActor* p){
  if ( p->GetSupplies()->count(num) == 1){
    return true;
  } else {
    return false;
  }
}




double Person::GetWorth(Good theGood){
  cout<<"Person::GetWorth() DONT CALL THIS "<<endl;
  double Priority2Money =0;
  double WantNumber2Money=1.5;

  int GoodNumber = theGood.GetGoodId();
  int Priority =theGood.GetPriority();

  int NumberOfPeopleThatWantTheGood=GoodManager::Get()->demand[GoodNumber];
  int NumberOfPeopleThatHaveTheGood=0;
  


  return Priority2Money*Priority + WantNumber2Money*NumberOfPeopleThatWantTheGood;
	   

}




/*
void Person::DoStep(){
  ///Loop over all connections and see if any of them have 
  ///things to buy or sell



  map<double,vector<double> > Profit2GoodNumberAndConnectionId_buy;
  map<double,vector<double> > Profit2GoodNumberAndConnectionId_sell;

  for (auto & connection : fConnections){

    for (auto & thisDemand : fDemands ){
      //Does this connection have this want

      if (connection.second->GetSupplies().count(thisDemand.first) !=0){
	//the connection has this want.  This person wants to buy it
	double Worth2Buyer=GoodManager::Get()->GetWorthToBuyer(this,thisDemand.first);
	double Worth2Seller=GoodManager::Get()->GetWorthToSeller(connection.second,thisDemand.first);
	
	
	if(Worth2Buyer >= Worth2Seller && Worth2Seller <= this->GetMoney()){
	  //Log the transaction where this person is buying the good from connection
	  vector <double> temp; 
	  temp.push_back(thisDemand.first);//The Good Id
	  temp.push_back(connection.first);//The Global Id of the connection
	  temp.push_back(Worth2Seller);//The Price

	  Profit2GoodNumberAndConnectionId_buy[-1*(Worth2Buyer-Worth2Seller)]=temp;
	  
	}//End if Worth2Buyer>=Worth2Seller
      }//End if connection has this good
    }//End loop over demands
    
    
    for (auto & thisSupply : fSupplies){
      //does this connection want this have
      if (connection.second->GetDemands().count(thisSupply.first)!=0){
	//the connection wants this have.  This person wants to sell it
	double Worth2Seller=GoodManager::Get()->GetWorthToSeller(this,thisSupply.first);
	double Worth2Buyer=GoodManager::Get()->GetWorthToBuyer(connection.second,thisSupply.first);


	if(Worth2Buyer >= Worth2Seller && Worth2Seller <= connection.second->GetMoney()){
	  //Do the transaction
	  vector <double> temp; 
	  temp.push_back(thisSupply.first);//The Good Id
	  temp.push_back(connection.first);//The Global Id of the connection
	  temp.push_back(Worth2Seller);
	  Profit2GoodNumberAndConnectionId_sell[-1*Worth2Seller]=temp;

	  
	  
	}//End if Worth2Buyer >= Worth2Seller
      }//end if connection has the good
    }//end loop over supplies 
  }//End loop ooer connections

  //now clean up.  

  
  auto i_sell = Profit2GoodNumberAndConnectionId_sell.begin();
  auto i_buy = Profit2GoodNumberAndConnectionId_buy.begin();

  double profitFromSell=-1*i_sell->first;
  double profitFromBuy =-1*i_buy->first;

  if (profitFromSell > profitFromBuy){
    //this person is removing a good and is geting money

    int good =(int)i_sell->second[0];
    int connection =(int)i_sell->second[1];
    //    cout<<"the sell connection is "<<connection<<" "<<i_sell->second[2]<<endl;
    this->RemoveAGood(good);
    fConnections[connection]->AddAGoodAndRemoveDemand(good);
    this->AddMoney(i_sell->second[2]);
    fConnections[connection]->SubtractMoney(i_sell->second[2]);
    
    (GoodManager::Get()->demand)[good]--;
    DataLogger::Get()->PushGoodPrice(good,i_sell->second[2],GoodManager::Get()->demand[good]);
  } else if (profitFromBuy > profitFromSell){
    //this person is getting a good.  And is losing money
    int good =(int)i_buy->second[0];
    int connection=(int)i_buy->second[1];
    //    cout<<"the Buy connection is "<<connection<<" "<<i_buy->second[2]<<endl; 
    this->AddAGoodAndRemoveDemand(good);
    fConnections[connection]->RemoveAGood(good);
    this->SubtractMoney(i_buy->second[2]);
    fConnections[connection]->AddMoney(i_buy->second[2]);

    (GoodManager::Get()->demand)[good]--;
    DataLogger::Get()->PushGoodPrice(good,i_buy->second[2],GoodManager::Get()->demand[good]);
  }

  //  int t;cin>>t;
}
*/
