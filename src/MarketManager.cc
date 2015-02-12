


#include "MarketManager.hh"
#include "Settings.hh"


//Initialize the singleton pointer

MarketManager * MarketManager::theManager=NULL;

MarketManager::MarketManager(){
  //Constructor
  rSellPrices.resize(Settings::MaxGoodNumber);

}

MarketManager::~MarketManager(){
  //Deconstructor

}

MarketManager * MarketManager::Get(){
  
  if (theManager== NULL){
    theManager=new MarketManager();
  }
  return theManager;

}


void MarketManager::PlaceSellOrder(int GoodNumber,int SellerId, int Quantity,double Price){

  (rSellPrices[GoodNumber])[Price]=OrderInfo(SellerId,Quantity);

}


void MarketManager::Dump(){
  
  for ( int i=0;i < rSellPrices.size();i++){
    cout<<"Good "<<i<<" ";
    
    map<double, OrderInfo> temp = rSellPrices[i];
    for (auto & i : temp ){
      cout<<i.first<<"["<<i.second.Quantity<<"](ID="<<i.second.SellerId<<") ";
    }
    cout<<endl;


  }
  
  

}
