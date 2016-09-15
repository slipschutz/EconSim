
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
  rInitialTopConectivity=20;
  rNumInteractingPeoplePerStep=Settings::NumberOfInteractionsPerStep;

  rNumberOfPeopleDeaths=0;
  rNumberOfCompanyDeaths=0;
  rNumberOfBirths=0;
  
  rNumCurrentCompanies=0;
}

EconomicActorManager::~EconomicActorManager(){
  

  for (auto i : rTheListOfActors){
    Death::Get()->AddDead(i.first,Calendar::DayNumber);
    delete i.second;

  }

  rTheListOfActors.clear();
  cout<<"There were "<<rNumberOfPeopleDeaths<<" People deaths"<<endl;
  cout<<"There were "<<rNumberOfCompanyDeaths<<" Company deaths"<<endl;
  cout<<"There were "<<rNumberOfBirths<<" births"<<endl;
}

void EconomicActorManager::PrintAllInfo(){
  for (auto i : rTheListOfActors){i.second->PrintInfo();
  }
}
void EconomicActorManager::Initialize(){


}


void EconomicActorManager::BuildList(int NumberOfPeople){
  rTheListOfActors.clear();//Clear the list 
  rNumPeople=NumberOfPeople;
 
  //rTheListOfActors.resize(NumberOfPeople);

  // for (int i=0;i<NumberOfPeople;i++){
  //   rTheListOfActors[i]->Initialize();
  //   int thisEntriesNumConnections = RandomManager::GetRand(rInitialTopConectivity);
  //   for (int j=0;j<thisEntriesNumConnections;j++){
  //     int n= RandomManager::GetRand(NumberOfPeople);
  //     if (n != i ){
  // 	rTheListOfActors[i]->MakeConnection(rTheListOfActors[n]);
  //     }
  //   }
  // }


}

void EconomicActorManager::BuildCompleteNetwork(int NumberOfActors){
  rNumPeople=NumberOfActors;
  //  rTheListOfActors.resize(NumberOfActors);
  
  for (int i=0;i<NumberOfActors;i++){
    EconomicActor * a = new Person(this);
    rTheListOfActors.insert(make_pair(a->GetBaseId(),a));
    rTheIds.push_back(a->GetBaseId());
  }

  ActorLogger::Get()->thePerson=rTheListOfActors.begin()->second->GetBaseId();

  // for (int i=0;i<0.5*NumberOfActors;i++){
  //   EconomicActor * a = new Manufacturer(1000,this);//Have these initial companies start with 1000 dollars
  //   rTheListOfActors.insert(make_pair(a->GetBaseId(),a));
  //   rTheIds.push_back(a->GetBaseId());
  // }

  rNumPeople=rTheListOfActors.size();
  cout<<"TOTAL NUMBER OF ACTORS "<<rNumPeople<<endl;
  if (rTheIds.size() !=rNumPeople){
    MessageException e("<EconomicActorManager::BuildCompleteNetwor> Length of id list does not match number of actors");
    throw e;
  }

  // for (int i=0;i<rTheListOfActors.size();i++){
  //   rTheListOfActors[i]->Initialize();
  //   for (int j=0;j<rTheListOfActors.size();j++){
  //     if (i != j){
  // 	rTheListOfActors[i]->MakeConnection(rTheListOfActors[j]);
  //     }
  //   }
  // }

  for (auto i : rTheListOfActors){
    i.second->Initialize();
  }
  
  // for (auto  i : rTheListOfActors){
  //   i.second->Initialize();
  //   for (auto j : rTheListOfActors){
  //     if (i.first != j.first){
  // 	i.second->MakeConnection(j.second);
  //     }
  //   }
  // }

}

void EconomicActorManager::BuildTestNetwork(){
  rNumPeople=2;
  

  Person * a = new Person(this);
  a->Initialize();
  rTheListOfActors.insert(make_pair(a->GetBaseId(),a));
  rTheIds.push_back(a->GetBaseId());

  ActorLogger::Get()->thePerson=rTheListOfActors.begin()->second->GetBaseId();


  EconomicActor * a1 = new Manufacturer(100000,this,a,0);//Have these initial companies start with 1000 dollars
  a1->Initialize();
  rTheListOfActors.insert(make_pair(a1->GetBaseId(),a1));
  rTheIds.push_back(a1->GetBaseId());

  
}


void EconomicActorManager::DoAStep(){
  cout<<"Size is "<<rTheListOfActors.size()<<endl;
  rTheIds.clear();
  rNumCurrentCompanies=0;
  for (auto i : rTheListOfActors){
    ActorActions a =i.second->BeginningOfStep();

    if ( i.second->GetActorType()== ActorTypes::Manufacturer){
      rNumCurrentCompanies++;
    }

    //Recreate the ids list
    rTheIds.push_back(i.first);

  }
  
  cout<<"FOR ToNY "<<rNumCurrentCompanies<<" "<<GetNumberOfPeople()<<endl;


  //RANDOMLY SORT THE LIST BEFOR EACH STEP
  std::random_shuffle ( rTheIds.begin(), rTheIds.end(),RandomManager::GetRand );

  
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
      }else{
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


  DataLogger::Get()->FinalizePrices();

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
  //add it to the connections of everyone in the network
  act->Initialize();
  rTheListOfActors.insert(make_pair(act->GetBaseId(),act));
  rTheIds.push_back(act->GetBaseId());
  


  // for (auto it : rTheListOfActors){

  //   if (it.second->GetBaseId() !=act->GetBaseId()){
  //     act->MakeConnection(it.second);
  //   }
  // }
  rNumberOfBirths++;
}

void EconomicActorManager::PrintConnections(){

  for (auto i: rTheListOfActors){
    i.second->DumpConnections();
  }

}

void EconomicActorManager::PrintHavesWants(){
  for (int i=0;i<rTheListOfActors.size();i++){
    cout<<"-------------------------------------"<<endl;
    //    rTheListOfActors[i]->DumpHavesWants();
    cout<<"-------------------------------------"<<endl;
  }
}

void EconomicActorManager::PrintHavesWants(int index){
  //rTheListOfActors[index]->DumpHavesWants();
}

void EconomicActorManager::PrintMoney(){
  for ( auto &i : rTheListOfActors){
    //   cout<<"Person "<<i->GetBaseId()<<" "<<i->GetMoney()<<endl;
  }

}
// void PersonManager::SetPersonToLog(int v){
//   rTheLogger->SetPerson(v);
// }

// PersonLogger* PersonManager::GetLogger(){
//   return rTheLogger;
// }

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
      //Confirmed that this is a person. Preform reinterpret cast
      return reinterpret_cast<Company*>(it->second);
    }else {
      s<<"<EconomicActorManager::FindCompany> id is not a company"<<endl;
      MessageException e(s.str());
      throw e;
    }
  }
}

