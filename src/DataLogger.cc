


#include "DataLogger.hh"

#include "TransactionRecord.hh"
#include "EconomicActor.hh"
#include "GoodManager.hh"

DataLogger * DataLogger::theLogger = NULL;


DataLogger::DataLogger() : pBufferSize(1000){

  pFileForGoodPrices.open("./data/GoodPrices.dat");
  pFileForEndingMoneyDistribution.open("./data/EndingMoneyDistribution.dat");

  rTempPrice=0;
  rTempNumTransactions=0;
  rTempVolume=0;


  rTempJobSalary=0;
  rTempNumJobPostings=0;

  

  
}


DataLogger::~DataLogger(){

  if (theGoodPrices.size() !=0){
    pWriteToDisk();
    theGoodPrices.clear();
  }

  pFileForGoodPrices.close();
  pFileForEndingMoneyDistribution.close();

}


DataLogger * DataLogger::Get(){
  if (theLogger == NULL){
    theLogger = new DataLogger();
  }

  return theLogger;
}



void DataLogger::PushGoodPrice(int GoodNumber,double price, int supply){

  //theGoodPrices.push_back(TransactionRecord(GoodNumber,price,supply));

  
  rTempPrice+=price;
  rTempNumTransactions++;
  rTempVolume+=supply;


  if (theGoodPrices.size() >= pBufferSize){
    //pWriteToDisk();
    //    theGoodPrices.clear();
  }

}

void DataLogger::FinalizePrices(){
  double avgPrice = rTempPrice/rTempNumTransactions;
  
  theGoodPrices.push_back(TransactionRecord(0,avgPrice,rTempVolume));


  theSalaries.push_back(rTempJobSalary/rTempNumJobPostings);

  theSupplies.push_back(GoodManager::Get()->supply[0]);
  theDemands.push_back(GoodManager::Get()->demand[0]);


  rTempPrice=0;
  rTempNumTransactions=0;
  rTempVolume=0;


  rTempJobSalary=0;
  rTempNumJobPostings=0;
    

}



void DataLogger::PushJobInfo(double salary){

  rTempJobSalary+=salary;
  rTempNumJobPostings++;


}

void DataLogger::pWriteToDisk(){
  
  for (auto & i : theGoodPrices){
    pFileForGoodPrices<<i.Price<<" "<<i.Supply<<endl;

  }


}



void DataLogger::LogEndingMoneyDistribution(unordered_map <int,EconomicActor*>* list){
  
  for (auto & i : *list){
    pFileForEndingMoneyDistribution<<i.second->GetActorType()<<" " <<i.second->GetMoney()<<endl;
  }


}
