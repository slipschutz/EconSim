
#include "EconomicActorManager.hh"
#include <algorithm>
#include <cstdlib>

#include "Person.hh"
#include "Manufacturer.hh"
#include "Retailer.hh"
#include "DeadActor.hh"
#include "ActorLogger.hh"

#include "RandomManager.hh"
#include "Settings.hh"
#include "MarketManager.hh"
#include "GoodManager.hh"

#include "Exceptions.hh"
#include "Calendar.hh"

EconomicActorManager::EconomicActorManager() {
  //  srand(0);
  rInitialTopConectivity=20;
  rNumInteractingPeoplePerStep=Settings::NumberOfInteractionsPerStep;

  rNumberOfDeaths=0;
  rNumberOfBirths=0;
  

}

EconomicActorManager::~EconomicActorManager(){
  

  for (auto i : rTheListOfActors){

    delete i.second;

  }

  rTheListOfActors.clear();
  cout<<"There were "<<rNumberOfDeaths<<" deaths"<<endl;
  cout<<"There were "<<rNumberOfBirths<<" births"<<endl;
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
    EconomicActor * a = new Person();
    rTheListOfActors.insert(make_pair(a->GetBaseId(),a));
    rTheIds.push_back(a->GetBaseId());
  }
  ActorLogger::Get()->thePerson=rTheListOfActors.begin()->second->GetBaseId();
  for (int i=0;i<0.1*NumberOfActors;i++){
    EconomicActor * a = new Manufacturer(1000);//Have these initial companies start with 1000 dollars
    rTheListOfActors.insert(make_pair(a->GetBaseId(),a));
    rTheIds.push_back(a->GetBaseId());
  }
  rNumPeople=rTheListOfActors.size();
  
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

  for (auto  i : rTheListOfActors){
    i.second->Initialize();
    for (auto j : rTheListOfActors){
      if (i.first != j.first){
	i.second->MakeConnection(j.second);
      }
    }
  }

}


void EconomicActorManager::DoAStep(){

  //First in each step call begin of step for all economic actors

  // for (int i=0;i<rTheListOfActors.size();i++){
  //   rTheListOfActors[i]->BeginningOfStep();
  // }


  // for (auto i : rTheListOfActors){
  //   i.second->PrintInfo();
  // }
  if (Calendar::DayNumber % 10 == 0){
    GoodManager::Get()->Dump();

  }

  vector <EconomicActor*> listOfNewActors;

  for (auto i : rTheListOfActors){
    ActorActions a =i.second->BeginningOfStep();
    if (a == ActorActions::StartedCompany && i.second->GetActorType()==ActorTypes::Person){
      double startup=reinterpret_cast<Person*>( i.second)->GetCompanyInvestment();
      i.second->SubtractMoney(startup);
      listOfNewActors.push_back(new Manufacturer(startup));
    }
  }
  
  for (auto & i : listOfNewActors){
    MakeActor(i);
  }


  // rTheListOfActors[0]->DumpSupplies();
  // rTheListOfActors[0]->DumpDemands();
  
  // GoodManager::Get()->Dump();
  //Each call to make transactions will pick some number
  //of random people and those people will have transactions
  
  //  MarketManager::Get()->Dump();

  // cout<<endl;
  // for (map<int,bool>::iterator ii = tempPersonMap.begin();ii!=tempPersonMap.end();
  //      ii++){
  //   rTheListOfActors[ii->first]->DoStep();
  // }

  //RANDOMLY SORT THE LIST BEFOR EACH STEP
  std::random_shuffle ( rTheIds.begin(), rTheIds.end(),RandomManager::GetRand );

  
  for (auto & i : rTheIds ){
    auto it=rTheListOfActors.find(i);
    if (it != rTheListOfActors.end()){
      it->second->DoStep();
    }
  }
  

  vector <int> ToBeKilled;
  //Call end of step for every actor in the list 
  for (auto & i : rTheListOfActors){

    if (i.second->EndOfStep()==ActorActions::Died ){

      //This actor died
      //Remove reference to it from everyone else 
      //Replace the pointer to a DeadActor object
      //This way we don't need to move the vector
      //IE don't need to call erase
      ToBeKilled.push_back(i.first);

      KillActor(i.second);
      delete i.second;

      //Make pointer point to an empty class
      
      // EconomicActor * a = new DeadActor();
      // rTheListOfActors.insert(make_pair(a->GetBaseId(),a));


    }
  }
  
  // for (int i=0;i<ToBeKilled.size();i++){
  //   cout<<ToBeKilled[i]<<" ";
  // }cout<<endl;


 
  for (auto i : ToBeKilled){
    rTheListOfActors.erase(i);
  }

}

void EconomicActorManager::KillActor(EconomicActor* act){
  //This method will remove all connections the actor has 
  int DeadMansBaseId=act->GetBaseId();

  //Loop over the connections and remove this actor from
  //the list of everyone else
  for ( auto & ii : (*act->GetConnections())){
    //    ii.first is the id of the connection
    //    ii.second should be pointer to connection
    ii.second->GetConnections()->erase(DeadMansBaseId);
  }

      rNumberOfDeaths++;
}

void EconomicActorManager::MakeActor(EconomicActor* act){
  //need to integrate this actor into all the lists and
  //add it to the connections of everyone in the network
  act->Initialize();
  rTheListOfActors.insert(make_pair(act->GetBaseId(),act));
  rTheIds.push_back(act->GetBaseId());
  
  for (auto it : rTheListOfActors){
    if (it.second->GetBaseId() !=act->GetBaseId()){
      act->MakeConnection(it.second);
    }
  }
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
