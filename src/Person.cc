#include "Person.hh"
#include "RandomManager.hh"
#include "ActorLogger.hh"
#include "GoodManager.hh"
#include "DataLogger.hh"
#include "MarketManager.hh"
#include "Calendar.hh"



#include "Company.hh"
#include "Manufacturer.hh"
#include "EconomicActorManager.hh"

int largestGoodNum=1;

Person::Person(EconomicActorManager* man, bool DoInitialize) : EconomicActor(man){
  rEmployer=NULL;

  // fSupplies.clear();
  // fDemands.clear();

  rHaveAJob=false;
  rEmployerId=-1;
  rWasFiredInPreviousStep=false;
  rWasPaidInPreviousStep=false;

  //Good ID 0 is food and should have the highest prioriry 
  fGoodPriorities[0]=Settings::MaxGoodPriority;


  if (DoInitialize){
    Initialize();
  }
}

Person::~Person(){
  //Need to clean up this economic actor 
  //
  if (rEmployer!=NULL){
    rEmployer->RemoveEmployee(this);
  }

}


double Person::GetCompanyInvestment(){

  return (rMyTraits.InvestmentLevel)*fMoney;

}
void Person::Initialize(){
  //Randomly Pick starting money 
  //  rMoney = RandomManager::GetRand(1000);

  fMoney =100000;
  AddSupply(0,100);

  //Initialize the traits for this person
  rMyTraits.InitializeTraits();


}

void Person::Initialize(Person *p){
  Initialize();
  
  //Initialize Person specific Traits 
  
  // rRestlessness=p->rRestlessness;
  // rGluttoness=p->rGluttoness;
  // rInvestmentLevel=p->rGluttoness;

}

void Person::CopyTraits(const PersonTraitPact & Traits2Copy){
  this->rMyTraits=Traits2Copy;
}
void Person::CopyAndMutateTraits(const PersonTraitPact & Traits2Copy){
  this->rMyTraits.MutateCopy(Traits2Copy);
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


void Person::GetPaid(double amount,string notes){

  this->AddMoney(amount);
  rWasPaidInPreviousStep=true;
  rPaidNotes<<notes<<endl;

}

ActorActions Person::BeginningOfStep(){

  ActorActions ret=ActorActions::None;

  //Check to see if this person has food (GOOD ID=0)
  if (fSupplies.count(0) == 0) {
    //Food is not in the supplies map
    AddSupply(0,0);//Make empty good
  }
  //  AddDemand(0,0);
  stringstream s;
  s<<"Dear Diary, \n Today is day "<<Calendar::DayNumber<<endl;






  //Add food Demand if in need of food.  Also has check on there not being any
  //demand already.  This prevents more demand to be added in later steps
  if (fSupplies[0].GetNumberOfCopies() < rMyTraits.FoodBuyingThreshold && 
      fDemands[0].GetNumberOfCopies()==0){
    //There are not enough copies of food in the supply
    int n=rMyTraits.Food2BuyAtOnce;
    AddDemand(0,n);//Add the demand for food
    s<<"I need Food.  I want to buy "<<n<<" foods"<<endl;
  }



  //look for Highest deamnd good and maybe start a company to make it
  int amtOfDemand=0;
  int HighestDemandGoodNum = GoodManager::Get()->FindHighestDemandGood(amtOfDemand);
  if (RandomManager::GetRand(10000)< 10 &&
      HighestDemandGoodNum!=-1 && amtOfDemand > 0){//10 here is to prevent too many companies from spawning 

    int theSupply=(*MarketManager::Get()->GetCurrentGoodsForSale()).at(HighestDemandGoodNum);
    
    int theDemand = GoodManager::Get()->demand[HighestDemandGoodNum];
    
    if (theDemand > 1.2*theSupply){
      
      double startup=this->GetCompanyInvestment();
      this->SubtractMoney(startup);

      Company * c =new Manufacturer(startup,fTheEconomicActorManager,this,HighestDemandGoodNum);
      s<<"Diary it is time i started a compnay it is a "<<c->GetBaseId()<<" i am investing "<<startup<<endl;
      rOwnedCompanies.push_back(c);
      fTheEconomicActorManager->MakeActor(c);
    }
  }

  
  //Randomly spawn more people
  if (RandomManager::GetRand(1000) < 2 &&     fSupplies[0].GetNumberOfCopies() >20){

    Person * aPerson = new Person(fTheEconomicActorManager,false);
    fTheEconomicActorManager->MakeActor(aPerson);
    aPerson->CopyAndMutateTraits(this->rMyTraits);

    double temp=fMoney/2.0;
    this->SubtractMoney(temp);
    aPerson->AddMoney(temp);
    
    s<<"I am reproducing.  I don't care what my mom says!!"<<endl;
    s<<"I am giving this beautiful child "<<temp<<" monies"<<endl;
    this->RemoveSupply(0,10);
    aPerson->AddSupply(0,10);
    //    aPerson->SetActorLogger(new ActorLogger(aPerson->GetBaseId()));
  }


  //
  //Message to keep track of whether this person got fired in previous step
  //
  if (rWasFiredInPreviousStep==true){
    rWasFiredInPreviousStep=false;
    s<<"Diary i was fired in yesterday"<<endl;
  }
  
  if (rWasPaidInPreviousStep==true){
    rWasPaidInPreviousStep=false;
    s<<rPaidNotes.str();
    rPaidNotes.str("");
  }


  //Log the information
  if (fMyActorLogger!=NULL){
    fMyActorLogger->LogBeforeStepState(this);
    fMyActorLogger->BeforeMessage(s.str());
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
    int Good2Buy=-1;
    int totalWant=-1;
    int AmountOfGoodIWant=-1;

    
    while (true && it != fDemandPriorities2GoodNum.begin()){
      //move iterator 1 back from the end to get last elment
      it--;
      Good2Buy = it->second;//this is the good number that has the highest priority
      totalWant= fDemands[Good2Buy].GetNumberOfCopies();
      AmountOfGoodIWant =totalWant;
      if (AmountOfGoodIWant!=0){
	break;
      }
    }

    while (AmountOfGoodIWant >0){

      OrderInfo info;
      int Seller=MarketManager::Get()->GetCheapestSeller(Good2Buy,info);

      // if (Calendar::DayNumber > 1000){
      // 	cout<<"-------> "<< Calendar::DayNumber <<" good "<<Good2Buy<<" seller "<<Seller<< endl;
      // 	cin.get();
      // }



      if (Seller == -1 ){// there is no seller 
	// cout<<"NO SELLER FOR "<<this->GetBaseId()<<endl;
	dayNotes<<"I wanted to buy "<<totalWant<<" of good "<<Good2Buy<<" but can't find seller "<<endl
		<<" at the end of the day I still want "<<AmountOfGoodIWant<<endl;
	AmountOfGoodIWant=-10;
	break;
      } else if (Seller !=-1 && info.Price*AmountOfGoodIWant > fMoney){
	//This price is always the current cheapest price
	//if I don't have enought money to buy what I want then 
	//Not sale
	dayNotes<<"I wanted to buy "<<info.Price*AmountOfGoodIWant<<" worth of good "<<Good2Buy<<endl
		<<"but I only have "<<fMoney<<endl;
	if (AmountOfGoodIWant > 5){
	  //if they demand alot of the good try lowering it until they can afford the good
	  AmountOfGoodIWant=floor(AmountOfGoodIWant/2);
	}else{
	  //They can't afford anything
	  break;
	}
      } else {//Can afford the good and someone is selling it
	if (AmountOfGoodIWant <= info.Quantity ){
	  //All of the remaining good can be bought from this seller

	  rDoTransaction(Good2Buy,AmountOfGoodIWant,info.Price,Seller);

	  double totalCost=(AmountOfGoodIWant)*info.Price;
	  dayNotes<<"Haza I bought "<<AmountOfGoodIWant<<" of good "<<Good2Buy<<" from "<<Seller<<" at a price of "<<endl
		  <<info.Price<<" totaling "<<totalCost<<endl;
	  AmountOfGoodIWant=0;
	} else {
	  int PartOfWhatIWant=info.Quantity; // Buying everything this seller can offer
	  rDoTransaction(Good2Buy,PartOfWhatIWant,info.Price,Seller);
	  //This seller can only provide part of the order
	  double totalCost=(PartOfWhatIWant)*info.Price;
	  AmountOfGoodIWant-=PartOfWhatIWant;
	  dayNotes<<"Less Haza I bought "<<info.Quantity<<" of good "<<Good2Buy<<" from "<<Seller<<" at a price of "<<endl
		  <<info.Price<<" totaling "<<totalCost<<endl;
	}//End buy part of demand
      }//End can afford and someone is selling
    }//end while loop 
  }//end if perosn wants something 

  //Check to see if this person is employed
  if (!rHaveAJob){
    //Get a job
    JobInfo jInfo= MarketManager::Get()->GetBestJob();
    if (jInfo.EmployerID==-1){
      //No jobs silently do nothing
    }else{
      rHaveAJob=true;//Now has job
      Company * theCompany = fTheEconomicActorManager->FindCompany(jInfo.EmployerID);
      MarketManager::Get()->BrokerJob(this,theCompany,jInfo.salary);
      rEmployer=theCompany;
      rEmployerId=jInfo.EmployerID;
      dayNotes<<"I Got a job working for "<<jInfo.EmployerID<<" at salary "<<jInfo.salary<<endl;
    }
  }


  if (fMyActorLogger!=NULL){
    fMyActorLogger->DuringMessage(dayNotes.str());
  }

    
}

ActorActions Person::EndOfStep(){
  //cout<<"IN end of step for "<<this->GetBaseId()<<" SUpply "<<fSupplies[0].GetNumberOfCopies()<<endl;
  stringstream s;
  //Preform End of step things
  ActorActions ret;
  if (fSupplies[0].GetNumberOfCopies() < rMyTraits.Gluttoness&&
      Settings::CanStarveToDeath){
    s<<"I have died :( "<<endl;
    
    //KillActor does not delete it right away.
    //it removes it from the 
    fTheEconomicActorManager->MarkForDeath(this);
    for (auto i : rOwnedCompanies){
      fTheEconomicActorManager->MarkForDeath(i);
    }//end for
    
    ret=ActorActions::Died;
  }else if(fSupplies[0].GetNumberOfCopies() > rMyTraits.Gluttoness){
    fSupplies[0].RemoveCopies(rMyTraits.Gluttoness);
    //    GoodManager::Get()->RemoveSupply(0,1);
    s<<"I ate "<<rMyTraits.Gluttoness<<" foods "<<endl;
    ret=ActorActions::None;
  }

  
  if (fMyActorLogger!=NULL){
    fMyActorLogger->LogAfterStepState(this);
    fMyActorLogger->EndMessage(s.str());
  }
  

  //Randomly add new demands
  if ( RandomManager::GetUniform() < rMyTraits.Restlessness){
    int n=RandomManager::GetRand(Settings::MaxGoodNumber);
    if (n!=0){
      AddDemand(n,RandomManager::GetRand(1000));
    }
    
  }
  
  return ret;
}

void Person::rDoTransaction(int Good2Buy,int AmountOfGoodIWant,double price,int Seller){
  double totalCost=(AmountOfGoodIWant)*price;
  //Move the money
  Company* theSeller =fTheEconomicActorManager->FindCompany(Seller);
  
  theSeller->AddMoney(totalCost);
  this->SubtractMoney(totalCost);
  
  //Remove the Supply from seller
  
  GoodManager::Get()->ReconcileTransaction(theSeller,this,Good2Buy,AmountOfGoodIWant);
  MarketManager::Get()->CleanUpOrder(Good2Buy,price,Seller,AmountOfGoodIWant);

}


void Person::PrintInfo(){
  PrintLine('v',30);
  cout<<"Info for <Person> Base id "<<this->GetBaseId()<<" actor type "<<this->GetActorType()<<endl;
  cout<<"Has Job? "<<rHaveAJob<<" employer "<<rEmployerId<<endl;
  cout<<"I have "<<fMoney<<" monies"<<endl;
  cout<<"I own "<<rOwnedCompanies.size()<<" companies"<<endl;
  for (auto i : rOwnedCompanies){
    cout<<"I own "<<i->GetBaseId()<<" it is a "<<i->GetActorType()<<endl;
  }
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
  //  int NumberOfPeopleThatHaveTheGood=0;
  


  return Priority2Money*Priority + WantNumber2Money*NumberOfPeopleThatWantTheGood;
	   

}

void Person::RemoveCompany(Company* c){

  for (unsigned int i=0;i< rOwnedCompanies.size();i++){
    if (rOwnedCompanies[i]->GetBaseId()==c->GetBaseId()){

      rOwnedCompanies.erase(rOwnedCompanies.begin()+i);
      break;
    }
  }

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
