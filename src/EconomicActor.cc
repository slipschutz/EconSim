
#include "EconomicActor.hh"

#include "Good.hh"

#include "RandomManager.hh"
#include "DataLogger.hh"
#include "GoodManager.hh"
#include "Settings.hh"
#include "MarketManager.hh"

#include "ActorLogger.hh"

EconomicActor::EconomicActor(EconomicActorManager* man) : fNumConnections(0),fMyActorLogger(NULL),fTheEconomicActorManager(man){

  fConnections.clear();
  fDemands.clear();
  fSupplies.clear();
  fGoodPriorities.clear();

  fMoney=0;
  
  fGoodPriorities.resize(Settings::MaxGoodNumber);
  fGoodDemandLevels.resize(Settings::MaxGoodNumber);
  
  for (auto & i : fGoodPriorities){
    i=RandomManager::GetRand(Settings::MaxGoodPriority - 4);//-4 to keep things away from 100%
  }

  //Fill the maps of Demands and supplies 
  //with goods that have 0 quantity.
  //this way Later checks won't have to look
  //to see if the demand/supply exists in the map
  //The Priorities though are not being set
  //Those will be set at latter times in the
  //sub classes
  for ( int i=0;i<Settings::MaxGoodNumber;i++){
    fSupplies[i]=Good(i,0,fGoodPriorities[i],GoodType::Supply);
    fDemands[i]=Good(i,0,fGoodPriorities[i],GoodType::Demand);
  }

}

EconomicActor::~EconomicActor(){
  //cout<<"IN ~EconomicActor()"<<endl;
  fConnections.clear();

  if (fDemands.size()!=0){
    for (auto & i : fDemands){
      i.second.Clear();
    }
  }
  if (fSupplies.size()!=0){
    for (auto & i: fSupplies){
      i.second.Clear();
    }
  }

  fDemands.clear();
  fSupplies.clear();
  fGoodPriorities.clear();
  fDemandPriorities2GoodNum.clear();

  
  fNumConnections=0;
  fMoney=0;

  if (fMyActorLogger!=NULL){
    delete fMyActorLogger;
  }
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

void EconomicActor::DumpSupplies(){
  PrintLine('v',30);
  cout<<"The Supplies for EconomicActor "<<this->GetBaseId()<<endl;
    for (auto & ii :fSupplies){
    cout<<"     Good "<<ii.first<<" copies "<<ii.second.GetNumberOfCopies()<<" priority "<<ii.second.GetPriority()<<endl;
  }
  PrintLine('^',30);
}
void EconomicActor::DumpDemands(){
  PrintLine('v',30);
  cout<<"The Demands for EconomicActor "<<this->GetBaseId()<<endl;
  for (auto & ii : fDemands ){
    cout<<"     Good "<<ii.first<<" copies "<<ii.second.GetNumberOfCopies()<<" priority "<<ii.second.GetPriority()<<endl;     
  }
  cout<<"The Priorities map"<<endl;
  for (auto & ii : fDemandPriorities2GoodNum){
    cout<<"     Priority "<<ii.first<<" GoodNum "<<ii.second<<endl;
  }

  PrintLine('^',30);
}
void EconomicActor::PrintInfo(){
  PrintLine('v',30);
  cout<<"Info for <EconomicActor> Base Id "<<this->GetBaseId()<<" it is a "<<this->GetActorType()<<endl;
  PrintLine('^',30);
}


void EconomicActor::CalculateDemandSupplyLevels(){


  int total=0;
  int totalSupply=0;

  for ( auto i : fDemands){
    double dem=i.second.GetNumberOfCopies();
    total+=dem;
    fGoodDemandLevels[i.first]=dem;
  }
  for (auto i : fSupplies){
    double sup=i.second.GetNumberOfCopies();
    totalSupply+=sup;
    fGoodSupplyLevels[i.first]=sup;
  }

  for (int i=0;i<fGoodSupplyLevels.size();i++){
    fGoodSupplyLevels[i]/=totalSupply;
    fGoodDemandLevels[i]/=total;;
  }

}

void EconomicActor::AddDemand(int GoodNumber,int copies){
  //Regardless of wether the actor has this demand already
  //Get the good from the GoodManager so that the total
  //Demand is updated properly

  //  Good temp =GoodManager::Get()->MakeDemand(GoodNumber,copies,this);




  //if the actor already demands this good just add the copies 
  auto theDemand = fDemands.find(GoodNumber);
  if (theDemand == fDemands.end()){//The demand wasn't there
    //    fDemands[GoodNumber]=temp;
    Good temp(GoodNumber,copies,fGoodPriorities[GoodNumber],GoodType::Demand);
    fDemands.insert(make_pair(GoodNumber,temp));
  }else {// the good was there
    theDemand->second.AddCopies(copies);
  }
  //Now put the demand in the priority map
  int tempNum = fGoodPriorities[GoodNumber];//temp.GetPriority();

  //need to make sure that this is not a second copy of the same thing
  //if one is adding demand for a good that is already demanded then
  //it does not need to in this map again.
  auto range = fDemandPriorities2GoodNum.equal_range(tempNum);
  for (auto it = range.first;it != range.second;it++){
    if (it->second ==GoodNumber){
      //It is already there
      return;
    }
  }
  //it made it through the search.  Therefore it the demand
  //is not already there.  Add it
  fDemandPriorities2GoodNum.insert(make_pair(tempNum,GoodNumber));
  
  
}

// void EconomicActor::fAddASupply(int GoodNumber){


// }


void EconomicActor::RemoveSupply(int GoodNumber,int Quantity){
  if (fSupplies.count(GoodNumber) == 0){
    cout<<"Trying to remove supply "<<GoodNumber<<" from "<<this->GetBaseId()<<" when supply isn't there"<<endl;
    throw 2;
  }
  
  auto theSupply = fSupplies.find(GoodNumber);
  if (theSupply->second.GetNumberOfCopies() < Quantity){
    cout<<"Trying to remove "<<Quantity <<" copies of "<<GoodNumber <<" from "<<this->GetBaseId()<<" but there are only "<<
      theSupply->second.GetNumberOfCopies()<<" it is a "<<this->GetActorType()<<endl;
    throw 3;
  }

  theSupply->second.RemoveCopies(Quantity);

  
}


void EconomicActor::AddSupply(int GoodNumber,int Quantity){
  
  if (fSupplies.count(GoodNumber) == 0){
    //The supply wasn't in the map so just add it
    Good temp(GoodNumber,Quantity,fGoodPriorities[GoodNumber],GoodType::Supply);
    //    fSupplies[GoodNumber]=temp;
    //    stlMap.insert( map< string, X >::value_type( "test", x ) );
    fSupplies.insert( map<int,Good>::value_type(GoodNumber,temp));
  }else {
    //The supply was in the map.  increase the quantity
    auto theSupply = fSupplies.find(GoodNumber);
    theSupply->second.AddCopies(Quantity);
  }
  

}
void EconomicActor::RemoveDemand(int GoodNumber,int Quantity){

  auto theDemand = fDemands.find(GoodNumber);
  
  if (theDemand == fDemands.end()){
    //The good is not in the map
    cout<<"Trying to remove demand "<<GoodNumber<< " from "<<this->GetBaseId()<<" but it is not there"<<endl;
    throw 3;
  }else {
    int temp = theDemand->second.GetNumberOfCopies();
    if ( temp < Quantity){
      cout<<"Trying to remove "<<Quantity <<" copies of "<<GoodNumber <<" form "<<this->GetBaseId()<<" but there are only "<<temp<<
	" it is a "<<this->GetActorType()<<endl;
      throw 3;
    }
    theDemand->second.RemoveCopies(Quantity);
  }

  //Also need to deal with the fDemandPriorities2GoodNum
  //If the demand quantity is 0 then remove the priorirty from
  //the list of priorities 
  if (fDemands[GoodNumber].GetNumberOfCopies()==0){
    //need to make sure that if there are two things with the same 
    //priority that we remove the correct one.  
    //Get the range of things with same prioriry
    int prior=fDemands[GoodNumber].GetPriority();

    auto range = fDemandPriorities2GoodNum.equal_range(prior);
    //range is a pair of iterators.  the first one points to the 
    //first element with the same key and the second is the end
    for (auto it = range.first;it != range.second;it++){
      if (it->second == GoodNumber){//This is the one
	fDemandPriorities2GoodNum.erase(it);
	break;
      }
    }
  }

}
