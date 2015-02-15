


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

  (rSellPrices[GoodNumber]).insert(make_pair(Price,OrderInfo(SellerId,Quantity,Price)));

}

int MarketManager::GetCheapestSeller(int GoodNumber,OrderInfo & Info){
  multimap<double,OrderInfo> m =rSellPrices[GoodNumber];

  if (m.size() ==0 ){//there is no seller :(
    return -1;
  }else{
    Info=m.begin()->second;
    return m.begin()->second.SellerId;
  }

}

void MarketManager::CleanUpOrder(int GoodNumber,double price, int SellerId,int quantity){
  
  multimap<double,OrderInfo> * theMap = &rSellPrices[GoodNumber];

  //Find the range of entries that have the price 
  auto range = theMap->equal_range(price);
  
  for (auto it = range.first;it!=range.second;it++){
    if (it->second.SellerId == SellerId){
      //This is the right one
      if ( it->second.Quantity > quantity){
	it->second.Quantity-=quantity;
      } else { //erase it 
	theMap->erase(it);
      }
      break;
    }
  }

}

void MarketManager::Dump(){
  
  for ( int i=0;i < rSellPrices.size();i++){
    cout<<"Good "<<i<<" ";
    
    multimap<double, OrderInfo> temp = rSellPrices[i];
    for (auto & i : temp ){
      cout<<i.first<<"["<<i.second.Quantity<<"](ID="<<i.second.SellerId<<") ";
    }
    cout<<endl;


  }
  
  

}
