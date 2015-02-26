
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
using namespace std;

class DataLogger 
{
public:

  virtual ~DataLogger();

  static DataLogger* theLogger;

  static DataLogger * Get();


  void PushGoodPrice(int GoodNumber,double price,int supply);
  
  void LogEndingMoneyDistribution( unordered_map <int,EconomicActor*>* list);
private:
  DataLogger();

  ofstream pFileForGoodPrices;
  ofstream pFileForEndingMoneyDistribution;
  
  vector <TransactionRecord> theGoodPrices;

  int pBufferSize;
  void pWriteToDisk();
};



#endif /* __DATALOGGER_HH */
