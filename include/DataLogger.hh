
#ifndef __DATALOGGER_HH
#define __DATALOGGER_HH

#include <map>
#include <vector>
#include <unordered_map>

#include <iostream>
#include <string>
#include <fstream>

class TransactionRecord;
class EconomicActor;
class MarketManager;
class GoodManager;

using namespace std;

class DataLogger 
{
public:

  virtual ~DataLogger();

  static DataLogger* theLogger;

  static DataLogger * Get();


  void PushGoodPrice(int GoodNumber,double price,int supply);

  void PushJobInfo(double salary);
  void FinalizePrices();

  void LogEndingMoneyDistribution( unordered_map <int,EconomicActor*>* list);
  
  void LogMarketState(MarketManager *,GoodManager*);

  void LogPopulation(int num);



  vector <TransactionRecord >* GetThePrices(){return &theGoodPrices;}
  
  vector <double> * GetTheSalaries(){return &theSalaries;}

  vector <int>  &GetNumberOfPeople(){return rNumberOfPeople;}

  vector <double> theSupplies;
  vector <double> theDemands;
  

private:
  DataLogger();

  ofstream pFileForGoodPrices;
  ofstream pFileForEndingMoneyDistribution;
  
  vector <TransactionRecord> theGoodPrices;

  vector < vector <int> > rDemands;
  vector < vector <int> > rSupplies;

  vector <double> theSalaries;

  vector <ofstream*> rFilesForSupply;
  vector <ofstream*> rFilesForDemand;

  ofstream rFileForPopulation;

  vector <int> rNumberOfPeople;
  
  int pBufferSize;
  void pWriteToDisk();
};



#endif /* __DATALOGGER_HH */
