
#ifndef __ECONOMICACTOR_HH
#define __ECONOMICACTOR_HH

#include "Base.hh"
#include "Good.hh"

#include "ActorTypes.hh"

#include "ErrorManager.hh"
#include <sstream>

#include <map>
#include <vector>
using namespace std;


//Foward Declerations
class EconomicActorManager;


enum class ActorActions{
  Died,
  None,
  StartedCompany
};
  

class EconomicActor : public Base {
public:
  EconomicActor(EconomicActorManager*);

 
  /**Virtual Functions
       
   */
  virtual ~EconomicActor();
  virtual void MakeConnection(EconomicActor *); /// Makes a connection betwen two EconomicActors
  virtual bool CheckIsConnected(EconomicActor*);/// checks connection between two EconomicActors
  virtual void DumpConnections();
  virtual void DumpSupplies();
  virtual void DumpDemands();

  virtual void PrintInfo();
  /** Abstract Methods 

   */
  //End of Step returns a bool indicating whether
  //that actor has died.  for people this will mean
  //they ran out of food.  for company's ran out of money
  //means there dead
  virtual ActorActions EndOfStep()=0;
  virtual ActorActions BeginningOfStep()=0;
  virtual void Initialize()=0;
  virtual void DoStep()=0;
  virtual bool CheckTransactionMatch(int,EconomicActor*)=0;
  virtual int  GetActorType()=0;



  

  




  map<int,Good> * GetDemands(){return &fDemands;}
  map<int,Good> * GetSupplies(){return &fSupplies;}
  
  multimap<int,int> * GetDemandPriorities(){return &fDemandPriorities2GoodNum;}
  
  inline long GetNumConnections(){return fNumConnections;}
  void IncrementConnections(){fNumConnections++;}
  inline map <int,EconomicActor*>* GetConnections(){return &fConnections;}



  
  double GetMoney(){return fMoney;}
  void SubtractMoney(double v){fMoney=fMoney-v;}
  void AddMoney(double v){fMoney=fMoney+v;}

  inline int GetGoodPriority(int GoodId){return fGoodPriorities[GoodId];}
  void SetGoodPriority(int GoodId,double priority){fGoodPriorities[GoodId]=priority;}

protected:


  int fNumConnections;
  map <int,EconomicActor*> fConnections;
  double fMoney;

  map <int,Good> fDemands;
  map <int,Good> fSupplies;


  //Multi Map of priorties 
  //Using a MultiMap to allow things to have the 
  //same priorities 
  multimap<int,int> fDemandPriorities2GoodNum;
    
  vector <int>   fGoodPriorities;

  EconomicActorManager * fTheEconomicActorManager;

public:
  void RemoveSupply(int GoodNumber,int Quantity);
  void AddSupply(int GoodNumber,int Quantity);
  void RemoveDemand(int GoodNumber,int Quantity);

  /**Add a Demand.  Should Get a Good object from the 
     Good Manager and put it into the fDemands map.
     It should also put that pirority in the DemandPriorities
     2GoodNumber map. */
public:
  void AddDemand(int GoodNumber,int copies);
  
  /**Add Supply should probably be an
     abstrct method.  For a Manufactuere Adding supply will
     come from out side of the network.  For a person
     Adding a supply should come from within the network
   */
  void fAddASupply(int GoodNumber){}
  

};
#endif
