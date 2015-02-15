
#ifndef __DATALOGGER_HH
#define __DATALOGGER_HH

#include <vector>
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
  
  void LogEndingMoneyDistribution( vector <EconomicActor*>* list);
private:
  DataLogger();

  ofstream pFileForGoodPrices;
  ofstream pFileForEndingMoneyDistribution;
  
  vector <TransactionRecord> theGoodPrices;

  int pBufferSize;
  void pWriteToDisk();
};



#endif /* __DATALOGGER_HH */
