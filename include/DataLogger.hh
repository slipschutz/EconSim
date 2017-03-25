
#ifndef __DATALOGGER_HH
#define __DATALOGGER_HH

#include <map>
#include <vector>
#include <unordered_map>

#include <iostream>
#include <string>
#include <fstream>

#include "TransactionRecord.hh"

class EconomicActor;
class MarketManager;
class GoodManager;

using namespace std;

class DataLogger 
{
public:

  DataLogger();
  virtual ~DataLogger();


  void PushGoodPrice(int GoodNumber,double price,int supply);

  void PushJobInfo(double salary);
  void FinalizePrices();

  void LogEndingMoneyDistribution( unordered_map <int,EconomicActor*>* list);
  
  void LogMarketState(MarketManager *,GoodManager*);

  void LogPopulation(int num);
  void LogManufacturerNumber(int num);


  vector <TransactionRecord >* GetThePrices(){return &theGoodPrices;}
  
  vector <double> * GetTheSalaries(){return &theSalaries;}

  vector <int>  &GetNumberOfPeople(){return rNumberOfPeople;}
  vector <int>  &GetNumberOfManufacturers(){return rNumberOfManufacturers;}

  vector <double> theSupplies;
  vector <double> theDemands;
  


private:


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
  vector <int> rNumberOfManufacturers;
  
  int pBufferSize;
  void pWriteToDisk();
};



#endif /* __DATALOGGER_HH */
