
#include "PersonManager.hh"
#include <cstdlib>
#include "Person.hh"
PersonManager::PersonManager() {
  srand(0);
  rInitialTopConectivity=10;
}

PersonManager::~PersonManager(){

}
void PersonManager::Initialize(){


}


void PersonManager::BuildList(int NumberOfPeople){
  rTheListOfPeople.clear();//Clear the list 

  rTheListOfPeople.resize(NumberOfPeople);

  for (int i=0;i<NumberOfPeople;i++){
    int thisEntriesNumConnections = rand() % rInitialTopConectivity;
    for (int j=0;j<thisEntriesNumConnections;j++){
      int n= rand() % NumberOfPeople;
      if (n != i ){
	rTheListOfPeople[i].MakeConnection(&rTheListOfPeople[n]);
      }
    }
  }


}


void PersonManager::PrintConnections(){
  int numPeople = rTheListOfPeople.size();
  for (int i=0;i<numPeople;i++){
    rTheListOfPeople[i].DumpConnections();
  }

}
