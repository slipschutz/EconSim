
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

  void PushJobInfo(double salary);

  void LogEndingMoneyDistribution( unordered_map <int,EconomicActor*>* list);

  void FinalizePrices();

  vector <TransactionRecord >* GetThePrices(){return &theGoodPrices;}
  
  vector <double> * GetTheSalaries(){return &theSalaries;}
  

  vector <double> theSupplies;
  vector <double> theDemands;
  

private:
  DataLogger();

  ofstream pFileForGoodPrices;
  ofstream pFileForEndingMoneyDistribution;
  
  vector <TransactionRecord> theGoodPrices;

  vector <double> theSalaries;

  
  double rTempPrice;
  long rTempNumTransactions;
  double rTempVolume;

  double rTempJobSalary;
  long rTempNumJobPostings;



  int pBufferSize;
  void pWriteToDisk();
};



#endif /* __DATALOGGER_HH */
