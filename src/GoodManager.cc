

#include "GoodManager.hh"
#include "Good.hh"
#include "EconomicActor.hh"
#include "RandomManager.hh"
#include "Settings.hh"

GoodManager * GoodManager::theManager=NULL;

GoodManager::GoodManager(){


}
GoodManager::~GoodManager(){

}


GoodManager* GoodManager::Get(){
  if (theManager == (GoodManager*)NULL){
    theManager = new GoodManager();
  }

  return theManager;

}




//int sigma=10;
Good GoodManager::MakeDemand(int id, int copies){
  if (demand.count(id) == 0){
    demand[id]=copies;
  }else{
    demand[id]+=copies;
  }
  Good g(id,copies);
  g.SetPriority(-1);//Priority must be set by each individual actor based on thier preferences
  return g;
}




Good GoodManager::MakeSupply(int id, int copies){
  if (supply.count(id) == 0){
    supply[id]=copies;
  }else{
    supply[id]+=copies;
  }
  Good g(id,copies);
  g.SetPriority(-1);//Priority must be set by each individual actor based on thier preferences
  return g;
}


void GoodManager::AddSupply(int id, int copies){
  auto s = supply.find(id);

  if (s == supply.end()){
    //something is wrong
    throw 3;
  }
  s->second = s->second + copies;
  
}
void GoodManager::AddDemand(int id, int copies){
  auto d = demand.find(id);
  
  if ( d == demand.end()){
    throw 3;
  }
  d->second = d->second + copies;
}


void GoodManager::RemoveSupply(int id, int copies){
  if (supply.count(id)== 0){
    //Something has gone wrong. The supply isn't there
    throw 3;
  }
  int theSupply=supply[id];
  
  if (theSupply - copies < 0 ){
    //Something else has gone wrong.  There aren't that many supplies
    throw 3;
  }
  
  supply[id]=theSupply-copies;

}
void GoodManager::RemoveDemand(int id, int copies){

  if (demand.count(id) == 0){
    //something has gone wrong. that demand isn't here
    throw 3;
  }
  int theDemand = demand[id];
  
  if (theDemand - copies < 0){
    //something has gone wrong.  There aren't enoughg demands to remove
    throw 3;
  }
  
  demand[id]= theDemand -copies;
}



Good GoodManager::ReplaceHave(int id, int copies){
  Good g(id,copies);
  //  g.SetPriority(RandomManager::GetGaus(Settings::MaxGoodPriority/2,sigma));
  return g;

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

  Good theGood = theEconomicActor->GetDemands()[GoodId];

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
  
  Good theGood = theEconomicActor->GetSupplies()[GoodId];
  
  double Priority = theGood.GetNormPriority();
 

  double val= (demand[GoodId]*Demand2Money+supply[GoodId]*Supply2Money)/Priority;

  if (val<0){
    return 0;
  }else{
    return val;
  }


}
