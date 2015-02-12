


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
  OrderInfo(int id,int q) : SellerId(id), Quantity(q){}
  OrderInfo() : SellerId(-1),Quantity(-1){}
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
  
  void Dump();
  
private:
  MarketManager();

  //For each good make a map of prices to GLOBAL ID
  vector < map <double,OrderInfo> > rSellPrices;
  
};



#endif /* __MARKETMANAGER_HH */
