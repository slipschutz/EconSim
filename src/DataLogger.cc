


#include "DataLogger.hh"

#include "TransactionRecord.hh"
#include "EconomicActor.hh"
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



void DataLogger::PushGoodPrice(int GoodNumber,double price, int supply){

  theGoodPrices.push_back(TransactionRecord(GoodNumber,price,supply));

  if (theGoodPrices.size() >= pBufferSize){
    pWriteToDisk();
    theGoodPrices.clear();
  }

}

void DataLogger::pWriteToDisk(){
  
  for (auto & i : theGoodPrices){
    pFileForGoodPrices<<i.Price<<" "<<i.Supply<<endl;

  }


}



void DataLogger::LogEndingMoneyDistribution(vector <EconomicActor*>* list){
  
  for (auto & i : *list){
    pFileForEndingMoneyDistribution<<i->GetActorType()<<" " <<i->GetMoney()<<endl;
  }


}
