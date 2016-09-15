
#ifndef __GOODMANAGER_HH
#define __GOODMANAGER_HH

#include <vector>
#include <iostream>
#include <string>
#include <map>
using namespace std;

class Good;
class EconomicActor;

#include "Base.hh"

class GoodManager : public Base {
public:


  virtual ~GoodManager();
  //Methods to Get the static singleton
  static GoodManager *theManager;
  static GoodManager * Get();

  void Initialize(){}

  void ReconcileTransaction(EconomicActor *Seller,EconomicActor *Buyer,int good,int quantity);

  //Maps from good number to total supply/total demand
  map<int,int> supply;
  map<int,int> demand;


  
  void Dump();


  //Oldish methods 
  double GetWorthToBuyer(EconomicActor * theEconomicActor,int GoodID);
  double GetWorthToSeller(EconomicActor * theEconomicActor,int GoodID);


  //Methods to add supplies or demands to the economy
  //These will add things to the map if they are not already there

  Good MakeDemand(int id, int copies,EconomicActor*);
  Good MakeSupply(int id, int copies,EconomicActor*);


  //These methods assume that the GOOD id is already in the maps
  //They change the values in the map
  void RemoveSupply(int id, int copies);
  void RemoveDemand(int id, int copies);

  void AddSupply(int id, int copies);
  void AddDemand(int id, int copies);

  
  int FindHighestDemandGood();
 
private:


  GoodManager();//This is a static singleton
};



#endif /* __GOODMANAGER_HH */
