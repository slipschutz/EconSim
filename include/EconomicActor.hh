
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


class EconomicActor : public Base {
public:
  EconomicActor();

 
  /**Virtual Functions
       
   */
  virtual ~EconomicActor();
  virtual void MakeConnection(EconomicActor *); /// Makes a connection betwen two EconomicActors
  virtual bool CheckIsConnected(EconomicActor*);/// checks connection between two EconomicActors
  virtual void DumpConnections();


  /** Abstract Methods 

   */
  virtual void EndOfStep()=0;
  virtual void BeginningOfStep()=0;
  virtual void Initialize()=0;
  virtual void DoStep()=0;
  virtual bool CheckTransactionMatch(int,EconomicActor*)=0;
  virtual int GetActorType()=0;



  

  void AddAGood(int GoodNumber);
  void AddAGoodAndRemoveDemand(int GoodNumber);
  void RemoveAGood(int GoodNumber);
  



  map<int,Good> GetDemands(){return fDemands;}
  map<int,Good> GetSupplies(){return fSupplies;}

  
  inline long GetNumConnections(){return fNumConnections;}
  void IncrementConnections(){fNumConnections++;}
  inline map <int,EconomicActor*>* GetConnections(){return &fConnections;}



  
  double GetMoney(){return fMoney;}
  void SubtractMoney(double v){fMoney=fMoney-v;}
  void AddMoney(double v){fMoney=fMoney+v;}

protected:


  int fNumConnections;
  map <int,EconomicActor*> fConnections;
  int fMoney;

  map <int,Good> fDemands;
  map <int,Good> fSupplies;

  
  vector <int>   fGoodPriorities;


  

};
#endif
