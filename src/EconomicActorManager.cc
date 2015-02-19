
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

EconomicActorManager::EconomicActorManager() {
  //  srand(0);
  rInitialTopConectivity=20;
  rNumInteractingPeoplePerStep=Settings::NumberOfInteractionsPerStep;

  rNumberOfDeaths=0;
}

EconomicActorManager::~EconomicActorManager(){
  


  for (auto i : rTheListOfActors){

    delete i.second;

  }

  rTheListOfActors.clear();
  cout<<"There were "<<rNumberOfDeaths<<" deaths"<<endl;
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
  }
  ActorLogger::Get()->thePerson=rTheListOfActors.begin()->second->GetBaseId();
  for (int i=0;i<1*NumberOfActors;i++){
    EconomicActor * a = new Manufacturer();
    rTheListOfActors.insert(make_pair(a->GetBaseId(),a));
  }
  rNumPeople=rTheListOfActors.size();
  
  for (int i=0;i<rTheListOfActors.size();i++){
    rTheListOfActors[i]->Initialize();
    for (int j=0;j<rTheListOfActors.size();j++){
      if (i != j){
  	rTheListOfActors[i]->MakeConnection(rTheListOfActors[j]);
      }
    }
  }

}


void EconomicActorManager::DoAStep(){

  //First in each step call begin of step for all economic actors

  // for (int i=0;i<rTheListOfActors.size();i++){
  //   rTheListOfActors[i]->BeginningOfStep();
  // }

  for (auto i : rTheListOfActors){
    i.second->BeginningOfStep();
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

  //  std::random_shuffle ( rTheListOfActors.begin(), rTheListOfActors.end(),RandomManager::GetRand );

  
  for (auto & i : rTheListOfActors ){
    i.second->DoStep();
  }

  // for (auto & i : rTheListOfActors){
  //   cout<<i->GetBaseId()<<"("<<i->GetActorType()<<") ";
  // }
  // cout<<endl;
  // int t;cin>>t;
  

  vector <int> ToBeKilled;
  //Call end of step for every actor in the list 
  for (auto i : rTheListOfActors){

    if (i.second->EndOfStep()){

      //This actor died
      //Remove reference to it from everyone else 
      //Replace the pointer to a DeadActor object
      //This way we don't need to move the vector
      //IE don't need to call erase
      KillActor(i.second);

      delete i.second;
      ToBeKilled.push_back(i.first);
      //Make pointer point to an empty class
      EconomicActor * a = new DeadActor();
      rTheListOfActors.insert(make_pair(a->GetBaseId(),a));

      rNumberOfDeaths++;
    }
  }
  
  cout<<"before "<<rTheListOfActors.size()<<" "<<ToBeKilled.size()<<endl;
  for (auto i : ToBeKilled){
    //  rTheListOfActors.erase(rTheListOfActors.begin()+i);
  }
  cout<<"after "<<rTheListOfActors.size()<<endl;


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
}

void EconomicActorManager::PrintConnections(){
  int numPeople = rTheListOfActors.size();
  for (int i=0;i<numPeople;i++){
    rTheListOfActors[i]->DumpConnections();
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
