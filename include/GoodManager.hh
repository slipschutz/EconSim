
#ifndef __GOODMANAGER_HH
#define __GOODMANAGER_HH

#include <vector>
#include <iostream>
#include <string>
#include <map>
using namespace std;

class Good;
class EconomicActor;

class GoodManager 
{
public:

  virtual ~GoodManager();

  static GoodManager *theManager;
  static GoodManager * Get();


  map<int,int> supply;
  map<int,int> demand;



  double GetWorthToBuyer(EconomicActor * theEconomicActor,int GoodID);
  double GetWorthToSeller(EconomicActor * theEconomicActor,int GoodID);


  
  Good MakeDemand(int id, int copies);
  Good MakeSupply(int id, int copies);

  void RemoveSupply(int id, int copies);
  void RemoveDemand(int id, int copies);

  void AddSupply(int id, int copies);
  void AddDemand(int id, int copies);

  Good ReplaceHave(int id, int copies);
 
private:
  

  GoodManager();//This is a static singleton
};



#endif /* __GOODMANAGER_HH */
