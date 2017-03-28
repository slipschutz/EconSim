
#include "EconomicActorManager.hh"
#include <algorithm>
#include <cstdlib>

#include "Person.hh"

#include "Manufacturer.hh"
#include "Retailer.hh"
#include "DeadActor.hh"
#include "Company.hh"

#include "ActorLogger.hh"

#include "RandomManager.hh"
#include "Settings.hh"
#include "MarketManager.hh"
#include "GoodManager.hh"
#include "DataLogger.hh"


#include "Exceptions.hh"
#include "Calendar.hh"
#include "Death.hh"

EconomicActorManager::EconomicActorManager() {
  //  srand(0);
  
  rTheDataLogger = new DataLogger();

  rTheMarketManager = new MarketManager(rTheDataLogger);
  

  rNumberOfPeopleDeaths=0;
  rNumberOfPeopleBirths=0;
  rNumberOfCompanyBirths=0;
  rNumberOfCompanyDeaths=0;
  rNumberOfBirths=0;
  rNumberOfManufacturers=0;
  rNumCurrentCompanies=0;
}

EconomicActorManager::~EconomicActorManager(){
  

  for (auto i : rTheListOfActors){
    Death::Get()->AddDead(i.first,Calendar::DayNumber);
    delete i.second;

  }

  delete rTheDataLogger;
  delete rTheMarketManager;

  rTheListOfActors.clear();

  cout<<"There were "<<rNumberOfPeopleDeaths<<" People deaths"<<endl;
  cout<<"There were "<<rNumberOfCompanyDeaths<<" Company deaths"<<endl;

  cout<<"There were "<<rNumberOfPeopleBirths<<" People   births"<<endl;
  cout<<"There were "<<rNumberOfCompanyBirths<<" Company births"<<endl;

  cout<<"There were "<<rNumberOfBirths<<" births"<<endl;

  }

void EconomicActorManager::PrintAllInfo(){

  for (auto i : rTheListOfActors){
    i.second->PrintInfo();
  }
}

void EconomicActorManager::Initialize(){


}



void EconomicActorManager::BuildCompleteNetwork(int NumberOfActors){


  rNumberOfPeople=NumberOfActors;
  Person * luckyPerson;  
  for (int i=0;i<NumberOfActors;i++){
    Person * a = new Person(this);
    if (i<20){
      a->SetActorLogger(new ActorLogger(a->GetBaseId()));
    }
    
    rTheListOfActors.insert(make_pair(a->GetBaseId(),a));
    rTheIds.push_back(a->GetBaseId());
    luckyPerson=a;
  }


  int numSeedCompanies=0.1*NumberOfActors;
  rNumberOfManufacturers=numSeedCompanies;
  for (int i=0;i<numSeedCompanies;i++){
    Manufacturer * m = new Manufacturer(100000,this,luckyPerson,0);
    m->SetActorLogger(new ActorLogger(m->GetBaseId()));
    this->MakeActor(m);
  }


  for (auto i : rTheListOfActors){
    i.second->Initialize();
  }
 
}

void EconomicActorManager::BuildTestNetwork(){
  rNumberOfPeople=2;
  

  Person * a = new Person(this);
  a->Initialize();
  rTheListOfActors.insert(make_pair(a->GetBaseId(),a));
  rTheIds.push_back(a->GetBaseId());




  EconomicActor * a1 = new Manufacturer(100000,this,a,0);//Have these initial companies start with 1000 dollars
  a1->Initialize();
  rTheListOfActors.insert(make_pair(a1->GetBaseId(),a1));
  rTheIds.push_back(a1->GetBaseId());

  
}


void EconomicActorManager::DoAStep(){

  rTheDataLogger->LogPopulation(rNumberOfPeople);
  rTheDataLogger->LogManufacturerNumber(rNumberOfManufacturers);

    
  //First thing to to is clear the ID list as people may have been killed at end
  //of last step
  rTheIds.clear();
  rNumCurrentCompanies=0;

  //Loop over the list of all actors and call thier begin of step methods
  for (auto i : rTheListOfActors){
    //ActorActions a =i.second->BeginningOfStep();
    i.second->BeginningOfStep();

    if ( i.second->GetActorType()== ActorTypes::Manufacturer){
      rNumCurrentCompanies++;
    }
    //Fill the list of Actor IDs
    rTheIds.push_back(i.first);
  }
  
  //Log the demand and supply for all the goods
  for (int i=0;i<Settings::MaxGoodNumber;i++){

    rTheDataLogger->PushSupply(i,GoodManager::Get()->supply[i]);
    rTheDataLogger->PushDemand(i,GoodManager::Get()->demand[i]);
  }



  //RANDOMLY SORT THE LIST BEFOR EACH STEP
  std::random_shuffle ( rTheIds.begin(), rTheIds.end(),RandomManager::GetRand );

  //Loop over the randomly shuffled list of IDs and for each ID Find the person
  //in the actor list and call it's DOSTEP method
  for (auto & i : rTheIds ){
    auto it=rTheListOfActors.find(i);
    if (it != rTheListOfActors.end()){
      it->second->DoStep();
    }
  }
  
  
  rToBeKilled.clear();//Make sure that this is empty
  
  //Call end of step for every actor in the list 
  for (auto & i : rTheListOfActors){
    i.second->EndOfStep();
  }
  
  //Need to clean up the list of actors 
  //and the list of IDs
  for (auto i : rToBeKilled){
    if (rTheListOfActors.count(i) !=0){
      if (rTheListOfActors[i]->GetActorType()==ActorTypes::Person){
	rNumberOfPeopleDeaths++;
	rNumberOfPeople--;
      }else if (rTheListOfActors[i]->GetActorType()==ActorTypes::Manufacturer){
	rNumberOfManufacturers--;
	rNumberOfCompanyDeaths++;
      }

      delete rTheListOfActors[i];
      rTheListOfActors.erase(i);
      Death::Get()->AddDead(i,Calendar::DayNumber);
    }
  }
  
  //Clear the id list.  It seems difficult 
  //to go through and find the ids of dead actors as they
  //are in a random order
  rTheIds.clear();


  //  rTheDataLogger->FinalizePrices();

  rTheMarketManager->ClearMarket();

}

void EconomicActorManager::MarkForDeath(EconomicActor* act){

  //This method will remove all connections the actor has 
  int DeadMansBaseId=act->GetBaseId();
  

  //Loop over the connections and remove this actor from
  //the list of everyone else
  // for ( auto & ii : (*act->GetConnections())){
  //   //    ii.first is the id of the connection
  //   //    ii.second should be pointer to connection
  //   ii.second->GetConnections()->erase(DeadMansBaseId);
  // }

  rToBeKilled.push_back(DeadMansBaseId);
  
}

void EconomicActorManager::MakeActor(EconomicActor* act){
  //need to integrate this actor into all the lists and

  //act->Initialize();

  rTheListOfActors.insert(make_pair(act->GetBaseId(),act));
  rTheIds.push_back(act->GetBaseId());

  
  int t =act->GetActorType();
  if ( t == ActorTypes::Company || t==ActorTypes::Manufacturer){
    rNumberOfCompanyBirths++;
    rNumberOfManufacturers++;
  }else{
    rNumberOfPeopleBirths++;
    rNumberOfPeople++;
  }

  rNumberOfBirths++;
}



void EconomicActorManager::PrintHavesWants(){
  for (unsigned int i=0;i<rTheListOfActors.size();i++){
    cout<<"-------------------------------------"<<endl;
    //    rTheListOfActors[i]->DumpHavesWants();
    cout<<"-------------------------------------"<<endl;
  }
}

void EconomicActorManager::PrintHavesWants(int index){
  //rTheListOfActors[index]->DumpHavesWants();
}



Person * EconomicActorManager::FindPerson(int id){
  auto it = rTheListOfActors.find(id);
  stringstream s;
  if (it == rTheListOfActors.end()){//There is no actor with this id
    s<<"<EconomicActorManager::FindPerson> connot find id "<<id<<endl;
    MessageException e(s.str());
    throw e;
  } else {
    if (it->second->GetActorType() == ActorTypes::Person){
      //Confirmed that this is a person. Preform reinterpret cast
      return reinterpret_cast<Person*>(it->second);
    }else {
      s<<"<EconomicActorManager::FindPerson> id is not a person"<<endl;
      MessageException e(s.str());
      throw e;
    }
  }
}

Company * EconomicActorManager::FindCompany(int id){
  auto it = rTheListOfActors.find(id);
  stringstream s;
  if (it == rTheListOfActors.end()){//There is no actor with this id
    Death::Get()->FindDead(id);
    s<<"<EconomicActorManager::FindCompnay> connot find id "<<id<<endl;
    MessageException e(s.str());
    throw e;
  } else {
    int t =it->second->GetActorType();
    if ( t == ActorTypes::Company || t==ActorTypes::Manufacturer){
      //Confirmed that this is a Company. Preform reinterpret cast
      return reinterpret_cast<Company*>(it->second);
    }else {
      s<<"<EconomicActorManager::FindCompany> id is not a company"<<endl;
      MessageException e(s.str());
      throw e;
    }
  }
}




vector<double> EconomicActorManager::GetPriceData(){
  vector <TransactionRecord>* theGoodPrices=  rTheDataLogger->GetThePrices();
  vector  <double> price;
  cout<<"The size is "<<theGoodPrices->size()<<endl;

  for ( auto i : *theGoodPrices){
    price.push_back(i.Price);
  }

  return price;
}


vector<int> EconomicActorManager::GetSupplies(int n){
  
  return rTheDataLogger->GetSupplyData(n);

}
vector<int> EconomicActorManager::GetDemands(int n){
  return rTheDataLogger->GetDemandsData(n);
}


vector<int> EconomicActorManager::GetPopulation(){

  return rTheDataLogger->GetNumberOfPeople();

}

vector<int> EconomicActorManager::GetNumManufacturers(){

  return rTheDataLogger->GetNumberOfManufacturers();

}





