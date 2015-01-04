
#ifndef __ECONOMICACTOR_HH
#define __ECONOMICACTOR_HH

#include "Base.hh"
#include "Good.hh"
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
  virtual  ~EconomicActor();
  virtual  void MakeConnection(EconomicActor *); // Makes a connection betwen two EconomicActors

  virtual void EndOfStep()=0;

  virtual void Initialize()=0;

  virtual void MakeTransactions()=0;
  virtual bool CheckTransactionMatch(int,EconomicActor*)=0;

  virtual bool CheckIsConnected(EconomicActor*);
  

  


  void IncrementConnections(){fNumConnections++;}

  void DumpConnections();

  
  inline long GetNumConnections(){return fNumConnections;}
  inline map <int,EconomicActor*>* GetConnections(){return &fConnections;}
  
  double GetMoney(){return fMoney;}
  void SubtractMoney(double v){fMoney=fMoney-v;}
  void AddMoney(double v){fMoney=fMoney+v;}

protected:


  int fNumConnections;
  map <int,EconomicActor*> fConnections;
  int fMoney;
  

};
#endif
