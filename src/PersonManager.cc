
#include "PersonManager.hh"
#include <cstdlib>
#include "Person.hh"
#include "PersonLogger.hh"
#include "RandomManager.hh"
#include "Settings.hh"

PersonManager::PersonManager() {
  srand(0);
  rInitialTopConectivity=20;
  rNumInteractingPeoplePerStep=Settings::NumberOfInteractionsPerStep;



  rTheLogger = new PersonLogger();
}

PersonManager::~PersonManager(){
  delete rTheLogger;

}
void PersonManager::Initialize(){


}


void PersonManager::BuildList(int NumberOfPeople){
  rTheListOfPeople.clear();//Clear the list 
  rNumPeople=NumberOfPeople;
  rTheListOfPeople.resize(NumberOfPeople);

  for (int i=0;i<NumberOfPeople;i++){
    rTheListOfPeople[i].Initialize();
    int thisEntriesNumConnections = RandomManager::GetRand(rInitialTopConectivity);
    for (int j=0;j<thisEntriesNumConnections;j++){
      int n= RandomManager::GetRand(NumberOfPeople);
      if (n != i ){
	rTheListOfPeople[i].MakeConnection(&rTheListOfPeople[n]);
      }
    }
  }


}

void PersonManager::BuildCompleteNetwork(int NumberOfPeople){
  rNumPeople=NumberOfPeople;
  rTheListOfPeople.resize(NumberOfPeople);
  
  for (int i=0;i<NumberOfPeople;i++){
    rTheListOfPeople[i].Initialize();
    for (int j=0;j<NumberOfPeople;j++){
      if (i != j){
	rTheListOfPeople[i].MakeConnection(&rTheListOfPeople[j]);
      }
    }
  }

}


void PersonManager::MakeTransactions(){
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
    rTheListOfPeople[ii->first].MakeTransactions();
  }

}


void PersonManager::PrintConnections(){
  int numPeople = rTheListOfPeople.size();
  for (int i=0;i<numPeople;i++){
    rTheListOfPeople[i].DumpConnections();
  }

}

void PersonManager::PrintHavesWants(){
  for (int i=0;i<rTheListOfPeople.size();i++){
    cout<<"-------------------------------------"<<endl;
    rTheListOfPeople[i].DumpHavesWants();
    cout<<"-------------------------------------"<<endl;
  }
}

void PersonManager::PrintHavesWants(int index){
    rTheListOfPeople[index].DumpHavesWants();
}

void PersonManager::PrintMoney(){
  for ( auto &i : rTheListOfPeople){
    cout<<"Person "<<i.GetBaseId()<<" "<<i.GetMoney()<<endl;
  }

}
void PersonManager::SetPersonToLog(int v){
  rTheLogger->SetPerson(v);
}

PersonLogger* PersonManager::GetLogger(){
  return rTheLogger;
}
