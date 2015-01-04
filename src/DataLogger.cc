


#include "DataLogger.hh"

#include "TransactionRecord.hh"

DataLogger * DataLogger::theLogger = NULL;


DataLogger::DataLogger() : pBufferSize(1000){

  pFileForGoodPrices.open("./data/GoodPrices.dat");
  pFileForEndingMoneyDistribution.open("./data/EndingMoneyDistribution.dat");

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



void DataLogger::PushGoodPrice(int GoodNumber,double price){

  theGoodPrices.push_back(TransactionRecord(GoodNumber,price));

  if (theGoodPrices.size() >= pBufferSize){
    pWriteToDisk();
    theGoodPrices.clear();
  }

}

void DataLogger::pWriteToDisk(){
  
  for (auto & i : theGoodPrices){
    pFileForGoodPrices<<i.Price<<endl;

  }


}
