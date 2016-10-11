


#include "DataLogger.hh"

#include "TransactionRecord.hh"
#include "EconomicActor.hh"
#include "MarketManager.hh"
#include "GoodManager.hh"
#include "Settings.hh"


DataLogger * DataLogger::theLogger = NULL;


DataLogger::DataLogger() : pBufferSize(1000){

  pFileForGoodPrices.open("./data/GoodPrices.dat");
  pFileForEndingMoneyDistribution.open("./data/EndingMoneyDistribution.dat");

  rFilesForSupply.resize(Settings::MaxGoodNumber);
  rFilesForDemand.resize(Settings::MaxGoodNumber);
  

  stringstream n;
  for (int i=0;i<Settings::MaxGoodNumber;i++){
    n.str("");
    n<<"./data/Supply"<<i<<".dat";
    rFilesForSupply[i]= new ofstream(n.str());

    n.str("");
    n<<"./data/Demand"<<i<<".dat";
    rFilesForDemand[i]= new ofstream(n.str());

  }
  rDemands.resize(Settings::MaxGoodNumber);
  rSupplies.resize(Settings::MaxGoodNumber);

  rFileForPopulation.open("./data/Population.data");
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

  // if (theGoodPrices.size() >= pBufferSize){
  //   pWriteToDisk();
  //   theGoodPrices.clear();
  // }

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


void DataLogger::LogMarketState(MarketManager *MarketMan,GoodManager * GoodMan){

  vector<int>* vec = MarketMan->GetCurrentGoodsForSale();

  for (unsigned int i=0;i< vec->size();i++){
    rSupplies[i].push_back((*vec)[i]);
  }

  for ( auto i : GoodMan->demand){
    rDemands[i.first].push_back(i.second);
  }


  if (rDemands[0].size() > 3 ){
    //Write everything to the files
    //loop over all goods 
    for (unsigned int i=0;i<rDemands.size();i++){
      
      for (auto j : rDemands[i]){
	(*rFilesForDemand[i])<<j<<endl;
      }

      for (auto j : rSupplies[i]){
	(*rFilesForSupply[i])<<j<<endl;
      }


    }
    
    rDemands.clear();
    rSupplies.clear();
    rDemands.resize(Settings::MaxGoodNumber);
    rSupplies.resize(Settings::MaxGoodNumber);
  }//end if writing to disk

}


void DataLogger::LogPopulation(int n){
  rNumberOfPeople.push_back(n);
  rFileForPopulation<<n<<endl;
}



void DataLogger::PushJobInfo(double salary){

}
void DataLogger::FinalizePrices(){

}
