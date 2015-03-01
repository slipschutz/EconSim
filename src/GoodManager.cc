

#include "GoodManager.hh"
#include "Good.hh"
#include "EconomicActor.hh"
#include "RandomManager.hh"
#include "Settings.hh"
#include "ActorTypes.hh"
#include "DataLogger.hh"
GoodManager * GoodManager::theManager=NULL;

GoodManager::GoodManager(){


}
GoodManager::~GoodManager(){

}


GoodManager* GoodManager::Get(){
  if (theManager == (GoodManager*)NULL){
    cout<<"GoodManager Made"<<endl;
    theManager = new GoodManager();
  }

  return theManager;

}




//int sigma=10;
Good GoodManager::MakeDemand(int id, int copies,EconomicActor* act){
  /*  if (demand.count(id) == 0){
    demand[id]=copies;
  }else{
    demand[id]+=copies;
  }
  Good g(id,copies);
  g.SetPriority(act->GetGoodPriority(id));//Priority must be set by each individual actor based on thier preferences
  return g;*/
  return Good();
}



Good GoodManager::MakeSupply(int id, int copies,EconomicActor* act){
  /*  if (act->GetActorType() != ActorTypes::Manufacturer){
    cout<<"ONLY Manufacturers can make goods"<<endl;
    throw 1;
  }

  if (supply.count(id) == 0){
    supply[id]=copies;
  }else{
    supply[id]+=copies;
  }
  Good g(id,copies);
  g.SetPriority(act->GetGoodPriority(id));//Priority must be set by each individual actor based on thier preferences
  return g;*/
  return Good();
}

void GoodManager::ReconcileTransaction(EconomicActor *Seller,EconomicActor *Buyer,int good,int quantity){
  //The Seller needs to have the goods removed from it's supplies and the buyer needs
  //to have the good removed from its demand

  Seller->RemoveSupply(good,quantity);
  Buyer->AddSupply(good,quantity);
  Buyer->RemoveDemand(good,quantity);
  


}

// void GoodManager::AddSupply(int id, int copies){
//   auto s = supply.find(id);

//   if (s == supply.end()){
//     //something is wrong
//     throw 3;
//   }
//   s->second = s->second + copies;
  
// }
// void GoodManager::AddDemand(int id, int copies){
//   auto d = demand.find(id);
  
//   if ( d == demand.end()){
//     throw 3;
//   }
//   d->second = d->second + copies;
// }


void GoodManager::RemoveSupply(int id, int copies){
  if (supply.count(id)== 0){
    //Something has gone wrong. The supply isn't there
    cout<<"<GoodManager::RemoveSupply> there is no supply with id "<<id<<endl;
    throw 3;
  }
  int theSupply=supply[id];
  
  if (theSupply - copies < 0 ){
    //Something else has gone wrong.  There aren't that many supplies
    cout<<"<GoodManager::RemoveSupply> there are not enough supplies"<<id<<"to remove "<<theSupply<<" vs "<<copies<<endl;
    throw 3;
  }
  
  supply[id]=theSupply-copies;

}
void GoodManager::RemoveDemand(int id, int copies){

  if (demand.count(id) == 0){
    //something has gone wrong. that demand isn't here
    cout<<"<GoodManager::RemoveDemand> there is no demand with id "<<id<<endl;
    throw 3;
  }
  int theDemand = demand[id];
  
  if (theDemand - copies < 0){
    //something has gone wrong.  There aren't enoughg demands to remove
    cout<<"<GoodManager::RemoveDemand> there are not enough demand "<<id<<" to remove "<<theDemand<<" vs "<<copies<<endl;
    throw 3;
  }
  
  demand[id]= theDemand -copies;
}

void GoodManager::AddSupply(int id, int copies){
  auto theSupply = supply.find(id);
  if (theSupply == supply.end()){
    //Supply is not in the map
    supply[id]=copies;
  }else {
    theSupply->second+=copies;
  }

}
void GoodManager::AddDemand(int id, int copies){
  auto theDemand=demand.find(id);
  if (theDemand == demand.end()){
    demand[id]=copies;
  }else{
    theDemand->second+=copies;
  }

}






/**Dump the total supply and demand 
 */
void GoodManager::Dump(){
  PrintLine('v',30);
  cout<<"Aggregate Supply"<<endl;
  for (auto i : supply ){

    cout<<"Good "<<i.first<<" supply level "<<i.second<<endl;
  }
  
  cout<<"Aggregate Demand"<<endl;
  for (auto i : demand ){

    cout<<"Good "<<i.first<<" demand level "<<i.second<<endl;
  }
  PrintLine('^',30);
}





double Demand2Money=(1.1/Settings::NumberOfPeople);
double Supply2Money=(-1.1/Settings::NumberOfPeople);
double Priority2Money=1.0;
double GoodManager::GetWorthToBuyer(EconomicActor * theEconomicActor,int GoodId){
  /** How much the person is willing to pay inorder to buy the good
      Should depend on:
      Overall demand
      this persons priority in getting the good
   */

  Good theGood = (*theEconomicActor->GetDemands())[GoodId];

  double Priority = theGood.GetNormPriority();
  
  //  cout<<"BUYER WORTH "<<demand[GoodId]<<" "<<supply[GoodId]<<" "<<Priority<<endl;
  double val=(demand[GoodId]*Demand2Money+supply[GoodId]*Supply2Money)/(Priority);

  if (val<0){
    return 0;
  }else{
    return val;
  }
}












double GoodManager::GetWorthToSeller(EconomicActor * theEconomicActor,int GoodId){
  /**How muh the person wants before they are willing to sell the good

   */
  
  Good theGood = (*theEconomicActor->GetSupplies())[GoodId];
  
  double Priority = theGood.GetNormPriority();
 

  double val= (demand[GoodId]*Demand2Money+supply[GoodId]*Supply2Money)/Priority;

  if (val<0){
    return 0;
  }else{
    return val;
  }


}
