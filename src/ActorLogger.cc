

#include "ActorLogger.hh"


#include "EconomicActor.hh"

#include <sstream>

ActorLogger * ActorLogger::theLogger=NULL;

ActorLogger * ActorLogger::Get(){
  if (theLogger == NULL){
    theLogger=new ActorLogger();
  }

  return theLogger;

}


void ActorLogger::LogBeforeStepState(EconomicActor* a){
  
  stringstream s;
  s<<"Supplies: "<<endl;

  for ( auto & i : *a->GetSupplies() ){
    s<<"   Good "<<i.first<<" ("<<i.second.GetNumberOfCopies()<<") priority "<<i.second.GetPriority()<<endl;
  }

  s<<"Demands "<<endl;
  for ( auto & i : *a->GetDemands() ){
    s<<"   Good "<<i.first<<" ("<<i.second.GetNumberOfCopies()<<") priority "<<i.second.GetPriority()<<endl;
  }

  s<<"Priortiy list "<<endl;
  for (auto & i : *a->GetDemandPriorities() ){
    s<<"   Priority "<<i.first<<" good "<<i.second<<endl;
  }

  s<<"Money "<<a->GetMoney()<<endl;
  
  rBeforeState.push_back(s.str());
}
void ActorLogger::LogAfterStepState(EconomicActor* a){
    stringstream s;
  s<<"Supplies: "<<endl;

  for ( auto & i : *a->GetSupplies() ){
    s<<"   Good "<<i.first<<" ("<<i.second.GetNumberOfCopies()<<") priority "<<i.second.GetPriority()<<endl;
  }

  s<<"Demands "<<endl;
  for ( auto & i : *a->GetDemands() ){
    s<<"   Good "<<i.first<<" ("<<i.second.GetNumberOfCopies()<<") priority "<<i.second.GetPriority()<<endl;
  }

  s<<"Money "<<a->GetMoney()<<endl;

  rAfterState.push_back(s.str());

}

void ActorLogger::DumpLog(){
  cout<<"This is the story of actor "<<thePerson<<endl;


  int size = rBeforeState.size();
  int size2 =rAfterState.size();


  if (size !=size2){
    cout<<"NO MATCH"<<endl;
    cout<<"Before "<<size<<endl;
    cout<<"after "<<size2<<endl;
    throw;
  }

  for (int i=0;i<size;i++){
    PrintLine('v',50);

    cout<<rBeforeMessage[i]<<endl;

    cout<<"At the start of the day I had"<<endl;
    cout<<rBeforeState[i]<<endl;

    cout<<rDuringMessage[i]<<endl;
    cout<<"At the end of the day I had"<<endl;
    cout<<rAfterState[i]<<endl;


    cout<<rAfterMessage[i]<<endl;

    PrintLine('^',50);
  }
}
