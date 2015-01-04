
#include "EconomicActor.hh"



#include "RandomManager.hh"

#include "DataLogger.hh"


EconomicActor::EconomicActor() : fNumConnections(0){
  
}

EconomicActor::~EconomicActor(){
  fConnections.clear();
  fNumConnections=0;
  fMoney=0;
}


void EconomicActor::MakeConnection(EconomicActor * p){
  if ( !CheckIsConnected(p) && !p->CheckIsConnected(this)){  

    fConnections[p->GetBaseId()]=p;
    (*(p->GetConnections()))[this->GetBaseId()]=this;

    this->IncrementConnections();
    p->IncrementConnections();
  } 
}

bool EconomicActor::CheckIsConnected(EconomicActor* p){
  if (this->fConnections.count(p->GetBaseId()) == 0){
    //IS NOT connected
    return false;
  } else {
    //IS connected
    return true;
  }

}

void EconomicActor::DumpConnections(){
  cout<<"Connections For EconomicActor "<<this->GetBaseId()<<endl;
  for (auto & ii : fConnections){
       
    cout<<"    EconomicActor "<<this->GetBaseId()<<" connects to "<<ii.first<<endl;
    
  }
}
