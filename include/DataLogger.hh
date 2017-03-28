
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


  /**Add a transcation record to the "theGoodPrices" vector
   */
  void PushGoodPrice(int GoodNumber,double price,int supply);

  void PushJobInfo(double salary);

  
  void LogMarketState(MarketManager *,GoodManager*);

  void LogPopulation(int num);
  void LogManufacturerNumber(int num);


  vector <TransactionRecord >* GetThePrices(){return &theGoodPrices;}
  
  vector <double> * GetTheSalaries(){return &theSalaries;}

  vector <int>  &GetNumberOfPeople(){return rNumberOfPeople;}
  vector <int>  &GetNumberOfManufacturers(){return rNumberOfManufacturers;}

  
  // vector <double> theSupplies;
  // vector <double> theDemands;
  
  void PushDemand(int GoodNumber,int value){
    rDemands[GoodNumber].push_back(value);
  }
  void PushSupply(int GoodNumber,int value){
    rSupplies[GoodNumber].push_back(value);
  }

  vector <int> & GetDemandsData(int GoodNumber){
    return rDemands[GoodNumber];
  }
    
  vector <int> & GetSupplyData(int GoodNumber){
    return rSupplies[GoodNumber];
  }
    
private:

  ofstream pFileForGoodPrices;
  ofstream rFileForPopulation;

  vector <TransactionRecord> theGoodPrices;

  vector < vector <int> > rDemands;
  vector < vector <int> > rSupplies;

  vector <double> theSalaries;

  vector <ofstream*> rFilesForSupply;
  vector <ofstream*> rFilesForDemand;




  /** Day by day number of people alive in sim
   */
  vector <int> rNumberOfPeople;
  /**Day by day number of Manufacturers in sim
   */
  vector <int> rNumberOfManufacturers;
  
  int pBufferSize;

};



#endif /* __DATALOGGER_HH */
