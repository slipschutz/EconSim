#include "Person.hh"
#include "RandomManager.hh"
#include "PersonLogger.hh"
#include "GoodManager.hh"
#include "DataLogger.hh"
int largestGoodNum=1;

Person::Person() {
  
}

Person::~Person(){


}



void Person::Initialize(){
  //Randomly Pick starting money 
  //  rMoney = RandomManager::GetRand(1000);
  fMoney =100000;
  int NumHaves=1;
  int NumWants=1;

  
  fSupplies.clear();
  fDemands.clear();
  // if(this->GetBaseId() == 6){ //Guy with all the stuff
  //   fSupplies[0]=GoodManager::Get()->MakeHave(RandomManager::GetRand(2),10);
  // } else {
  //   rWants[0]=GoodManager::Get()->MakeWant(0,400);
  // }

  fSupplies[0]=GoodManager::Get()->MakeSupply(0,RandomManager::GetRand(8)+1);
  fDemands[0]=GoodManager::Get()->MakeDemand(0,RandomManager::GetRand(800)+1);

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

  /*
  for (auto & ii : Profit2GoodNumberAndConnectionId_sell){
    cout<<"Person "<<this->GetBaseId()<<
      " Sells "<<ii.second[0]<<" from "<<ii.second[1]<<" at Price "<<ii.second[2]<<" profit "<<-1*ii.first<<endl;
  }
  cout<<"&&&&&&&&&&&&&"<<endl;

  for (auto & ii : Profit2GoodNumberAndConnectionId_buy){
    cout<<"Person "<<this->GetBaseId()<<
      " buys "<<ii.second[0]<<" from "<<ii.second[1]<<" at Price "<<ii.second[2]<<" profit "<<-1*ii.first<<endl;
  }
  cout<<"&&&&&&&&&&&&&"<<endl;


  cout<<"Profits "<<profitFromSell << " " <<profitFromBuy<<endl;
  */
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
  if ( p->GetSupplies().count(num) == 1){
    return true;
  } else {
    return false;
  }
}



void Person::EndOfStep(){
  //Preform End of step things

  
  //increment the wants priorities
  for (map<int,Good>::iterator ii = fDemands.begin();ii!=fDemands.end();ii++){
    ii->second.IncrementPriority();
  }
  
  ///Randomly add wants 
  
  if (RandomManager::GetRand(101)>90){
    int newWant= RandomManager::GetRand(largestGoodNum);
    if (fDemands.count(newWant)==0){
      Good aWant;
      aWant.SetGoodId(newWant);
      aWant.SetPriority(RandomManager::GetRand(101));
      fDemands[newWant]=aWant;
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
