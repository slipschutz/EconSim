
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
  
  void LogEndingMoneyDistribution( unordered_map <int,EconomicActor*>* list);
  
  void LogMarketState(MarketManager *,GoodManager*);

  void LogPopulation(int );
  
private:
  DataLogger();

  ofstream pFileForGoodPrices;
  ofstream pFileForEndingMoneyDistribution;
  
  vector <TransactionRecord> theGoodPrices;

  vector < vector <int> > rDemands;
  vector < vector <int> > rSupplies;

  vector <ofstream*> rFilesForSupply;
  vector <ofstream*> rFilesForDemand;

  ofstream rFileForPopulation;

  int pBufferSize;
  void pWriteToDisk();
};



#endif /* __DATALOGGER_HH */
