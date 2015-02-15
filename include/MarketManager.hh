


#ifndef __MARKETMANAGER_HH
#define __MARKETMANAGER_HH

#include <vector>
#include <map>
#include <iostream>
#include <string>
using namespace std;

class OrderInfo{
public:
  int SellerId;
  int Quantity;
  double Price;
  OrderInfo(int id,int q,double p) : SellerId(id), Quantity(q),Price(p){}
  OrderInfo() : SellerId(-1),Quantity(-1),Price(-1){}
};

/**This class will be responsible for holding by and sell orders from companies and 
   maybe people too?

   Will be a static singleton
 */
class MarketManager 
{
public:
  virtual ~MarketManager();
  
  static MarketManager * theManager;
  static MarketManager * Get();

  void PlaceSellOrder(int GoodNumber,int SellerID, int Quantity,double Price);
  
  int GetCheapestSeller(int GoodNumber,OrderInfo & Info);

  void CleanUpOrder(int GoodNumber,double price, int SellerId,int quantity);
 
  void ClearMarket(){
    for (auto & i : rSellPrices){
      i.clear();
    }
  }
  void Dump();
  
private:
  MarketManager();

  //For each good make a map of prices to GLOBAL ID
  vector < multimap <double,OrderInfo> > rSellPrices;
  
};



#endif /* __MARKETMANAGER_HH */
