
#include "EconomicActor.hh"

#include "RandomManager.hh"

#include "DataLogger.hh"
#include "GoodManager.hh"

EconomicActor::EconomicActor() : fNumConnections(0){
  fConnections.clear();
  fDemands.clear();
  fSupplies.clear();
  fGoodPriorities.clear();
}

EconomicActor::~EconomicActor(){
  fConnections.clear();
  fDemands.clear();
  fSupplies.clear();
  fGoodPriorities.clear();
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




void EconomicActor::AddAGood(int GoodNumber){
  //Add a good 
  //Get the good from the good managner so that the supply gets updated
  if (fSupplies.count(GoodNumber)==0){
    fSupplies[GoodNumber]=GoodManager::Get()->MakeSupply(GoodNumber,1);
    //cout<<"Adding good "<<GoodNumber<<" to rHaves for person "<<this->GetBaseId()<<endl;
  }else {
    //The good is already there person now has a second copy of the good
    int t=fSupplies[GoodNumber].GetNumberOfCopies() + 1;
    GoodManager::Get()->AddSupply(GoodNumber,1);
    fSupplies[GoodNumber].SetNumberOfCopies(t);
  }
}



void EconomicActor::RemoveAGood(int GoodNumber){
  if (fSupplies.count(GoodNumber)!=0){
    int t = fSupplies[GoodNumber].GetNumberOfCopies();
    if ( t ==1){
      fSupplies.erase(GoodNumber);
    } else {
      fSupplies[GoodNumber].SetNumberOfCopies(t-1);
    }
  } else {
    ///The good isn't there.  Silently do nothing
  }
}





void EconomicActor::AddAGoodAndRemoveDemand(int GoodNumber){
  AddAGood(GoodNumber);
  if (fDemands.count(GoodNumber) == 0 ){
    //Something is wrong.  Person doesn't want this good
    throw 1;
  }
  int t =fDemands[GoodNumber].GetNumberOfCopies();
  if (t == 1){
    fDemands.erase(GoodNumber);
  } else{
    fDemands[GoodNumber].SetNumberOfCopies(t-1);
  }
}
