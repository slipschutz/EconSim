
#include "EconomicActorManager.hh"
#include <cstdlib>
#include "Person.hh"

#include "RandomManager.hh"
#include "Settings.hh"

EconomicActorManager::EconomicActorManager() {
  srand(0);
  rInitialTopConectivity=20;
  rNumInteractingPeoplePerStep=Settings::NumberOfInteractionsPerStep;


}

EconomicActorManager::~EconomicActorManager(){


}
void EconomicActorManager::Initialize(){


}


void EconomicActorManager::BuildList(int NumberOfPeople){
  rTheListOfActors.clear();//Clear the list 
  rNumPeople=NumberOfPeople;
  rTheListOfActors.resize(NumberOfPeople);

  for (int i=0;i<NumberOfPeople;i++){
    rTheListOfActors[i]->Initialize();
    int thisEntriesNumConnections = RandomManager::GetRand(rInitialTopConectivity);
    for (int j=0;j<thisEntriesNumConnections;j++){
      int n= RandomManager::GetRand(NumberOfPeople);
      if (n != i ){
	rTheListOfActors[i]->MakeConnection(rTheListOfActors[n]);
      }
    }
  }


}

void EconomicActorManager::BuildCompleteNetwork(int NumberOfPeople){
  rNumPeople=NumberOfPeople;
  rTheListOfActors.resize(NumberOfPeople);
  
  for (int i=0;i<NumberOfPeople;i++){
    rTheListOfActors[i]->Initialize();
    for (int j=0;j<NumberOfPeople;j++){
      if (i != j){
	rTheListOfActors[i]->MakeConnection(rTheListOfActors[j]);
      }
    }
  }

}


void EconomicActorManager::MakeTransactions(){
  //Each call to make transactions will pick some number
  //of random people and those people will have transactions

  map <int,bool> tempPersonMap;

  while (tempPersonMap.size() < rNumInteractingPeoplePerStep){
    int n = RandomManager::GetRand(rNumPeople);
    if (tempPersonMap.count(n) == 0 ){
      tempPersonMap[n]=true;
    }
  }
  // cout<<endl;
  for (map<int,bool>::iterator ii = tempPersonMap.begin();ii!=tempPersonMap.end();
       ii++){
    rTheListOfActors[ii->first]->DoStep();
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
    cout<<"Person "<<i->GetBaseId()<<" "<<i->GetMoney()<<endl;
  }

}
// void PersonManager::SetPersonToLog(int v){
//   rTheLogger->SetPerson(v);
// }

// PersonLogger* PersonManager::GetLogger(){
//   return rTheLogger;
// }
