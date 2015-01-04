

#include "GoodManager.hh"
#include "Good.hh"
#include "Person.hh"
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


int sigma=10;
Good GoodManager::MakeWant(int id, int copies){
  if (demand.count(id) == 0){
    demand[id]=copies;
  }else{
    demand[id]+=copies;
  }
  Good g(id,copies);
  g.SetPriority(RandomManager::GetGaus(Settings::MaxGoodPriority/2,sigma));
  return g;
}

Good GoodManager::MakeHave(int id, int copies){
  if (supply.count(id) == 0){
    supply[id]=copies;
  }else{
    supply[id]+=copies;
  }
  Good g(id,copies);
  g.SetPriority(RandomManager::GetGaus(Settings::MaxGoodPriority/2,sigma));
  return g;
}


Good GoodManager::ReplaceHave(int id, int copies){
  Good g(id,copies);
  g.SetPriority(RandomManager::GetGaus(Settings::MaxGoodPriority/2,sigma));
  return g;

}


double Demand2Money=(1.1/Settings::NumberOfPeople);
double Supply2Money=(-1.1/Settings::NumberOfPeople);
double Priority2Money=1.0;
double GoodManager::GetWorthToBuyer(Person * thePerson,int GoodId){
  /** How much the person is willing to pay inorder to buy the good
      Should depend on:
      Overall demand
      this persons priority in getting the good
   */

  Good theGood = thePerson->GetWants()[GoodId];

  double Priority = theGood.GetNormPriority();
  
  //  cout<<"BUYER WORTH "<<demand[GoodId]<<" "<<supply[GoodId]<<" "<<Priority<<endl;
  double val=(demand[GoodId]*Demand2Money+supply[GoodId]*Supply2Money)/(Priority);

  if (val<0){
    return 0;
  }else{
    return val;
  }
}


double GoodManager::GetWorthToSeller(Person * thePerson,int GoodId){
  /**How muh the person wants before they are willing to sell the good

   */
  
  Good theGood = thePerson->GetHaves()[GoodId];
  
  double Priority = theGood.GetNormPriority();
 

  double val= (demand[GoodId]*Demand2Money+supply[GoodId]*Supply2Money)/Priority;

  if (val<0){
    return 0;
  }else{
    return val;
  }


}
