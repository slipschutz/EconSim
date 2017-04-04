

#include "DataLogger.hh"


#include "MarketManager.hh"
#include "Settings.hh"
#include "EconomicActor.hh"
#include "Company.hh"
#include "Person.hh"
#include "Exceptions.hh"
//Initialize the singleton pointer


MarketManager::MarketManager(DataLogger *d): rTheDataLogger(d)
{
  //Constructor
  rSellPrices.resize(Settings::MaxGoodNumber);
  rCurrentGoodsForSale.resize(Settings::MaxGoodNumber);
  rAveragePrice=0;
  rNumberOfDailySales=1;

}

MarketManager::~MarketManager(){
  //Deconstructor

}
void MarketManager::ClearMarket(){
  for (auto & i : rSellPrices){
    i.clear();
  }
  rJobListings.clear();
  rSellPrices.clear();
  rSellPrices.resize(Settings::MaxGoodNumber);

  rCurrentGoodsForSale.clear();
  //The Current goods for sale needs to zero-ed
  rCurrentGoodsForSale.resize(Settings::MaxGoodNumber,0);


  double temp=0;
  if (rNumberOfDailySales !=0){
    temp=rAveragePrice/rNumberOfDailySales;
  }
  rTheDataLogger->PushGoodPrice(0,temp,rNumberOfDailySales);
  rAveragePrice=0;
  rNumberOfDailySales=0;


}

 

void MarketManager::PlaceSellOrder(int GoodNumber,int SellerId, int Quantity,double Price){

  multimap <double,OrderInfo> * theMap=&rSellPrices[GoodNumber];
  auto range = theMap->equal_range(Price);

  for (auto it =range.first;it!=range.second;it++){
    if (it->second.SellerId==SellerId){
      //This seller is already in the map?

      stringstream s;s<<"This seller "<<SellerId<<" already has sellorder for "<<GoodNumber<<endl;
      MessageException e(s.str());
      //throw e;
      return;
    }
  }

  (rSellPrices[GoodNumber]).insert(make_pair(Price,OrderInfo(SellerId,Quantity,Price)));
  
  //Put the inforamtion from the sale in the list of current goods for sale
  //this will keep track of the FOR SALE supply of things 

  rCurrentGoodsForSale[GoodNumber]+=Quantity;


}

int MarketManager::GetCheapestSeller(int GoodNumber,OrderInfo & Info){
  multimap<double,OrderInfo> m =rSellPrices.at(GoodNumber);

  if (m.size() ==0 ){//there is no seller :(
    return -1;
  }else{
    Info=m.begin()->second;
    return m.begin()->second.SellerId;
  }

}

void MarketManager::CleanUpOrder(int GoodNumber,double price, int SellerId,int quantity){
  
  if (GoodNumber==0){
    rAveragePrice+=price;
    rNumberOfDailySales++;
  }

  ///Need to remove the order from the list of SellPrices
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

  ///Lower the Current Goods For Sale 
  rCurrentGoodsForSale[GoodNumber]-=quantity;

}

void MarketManager::PlaceJobPosting(double salary, int EmployerId){
  //JobInfo(Id,salary)
  rJobListings.insert(make_pair(salary,JobInfo(EmployerId,salary)));

}

void MarketManager::BrokerJob(Person * employee, Company * employer,double salary){
  rTheDataLogger->PushJobInfo(salary);


  employer->AddEmployee(employee,salary);
  //Now do clean up of job posting
  
  auto range = rJobListings.equal_range(salary);
  
  for (auto i = range.first;i!=range.second;i++){
    if (i->second.EmployerID ==employer->GetBaseId()){
      //This is the one or an identical copy of the 
      //one
      rJobListings.erase(i);
      break;
    }
  }

}
JobInfo MarketManager::GetBestJob(){
  if(rJobListings.size()==0){
    return JobInfo(-1,0);
  }
  auto it= rJobListings.end();
  it--;
  
  double s=it->first;
  int id =it->second.EmployerID;
  //JobInfo id salary
  return JobInfo(id,s);

}

void MarketManager::Dump(){
  cout<<"Begin MarketManager::Dump()"<<endl;

  for (unsigned int i=0;i < rSellPrices.size();i++){
    cout<<"Good "<<i<<" ";
    
    multimap<double, OrderInfo> temp = rSellPrices[i];
    for (auto & i : temp ){
      cout<<i.first<<"["<<i.second.Quantity<<"](ID="<<i.second.SellerId<<") ";
    }
    cout<<endl;
  }
  
  for (auto & i : rJobListings){
    cout<<"Salary "<<i.first<<" from "<<i.second.EmployerID<<endl;
  }
  cout<<"End MarketManager::Dump()"<<endl;
}


void MarketManager::DumpCurrentGoodsForSale(){


  // for (unsigned int i=0;i<rCurrentGoodsForSale.size();i++){
  //   cout<<"Good "<<i<<" has supply for sale "<<rCurrentGoodsForSale.at(i)<<endl;
    
  // }

}
