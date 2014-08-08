
#include "PersonLogger.hh"
#include "Person.hh"
#include "PersonSnapshot.hh"
#include "Good.hh"
#include <iomanip>
PersonLogger::PersonLogger(): rThePerson(-1){

  Initialize();
}

PersonLogger::~PersonLogger(){

}


void PersonLogger::Initialize(){
  rWantsSnapshots.clear();
  rHasSnapshots.clear();
  rGoodBeingTraded.clear();
  rValue2Buyer.clear();
  rValue2Seller.clear();
  rMoneyLog.clear();
}

void PersonLogger::CheckAndLogTransaction(Person* pWants,Person* pHas,int GoodBeingTraded,double value2Buyer,double value2Seller){

  if (pWants->GetBaseId() == rThePerson || pHas->GetBaseId()==rThePerson){
    PersonSnapshot WantsSnapshot;
    PersonSnapshot HasSnapshot;

    WantsSnapshot.personBaseId= pWants->GetBaseId();
    WantsSnapshot.wants = pWants->GetWants();
    WantsSnapshot.haves = pWants->GetHaves();
    rWantsSnapshots.push_back(WantsSnapshot);

    HasSnapshot.personBaseId= pHas->GetBaseId();
    HasSnapshot.wants = pHas->GetWants();
    HasSnapshot.haves = pHas->GetHaves();
    rHasSnapshots.push_back(HasSnapshot);

    rGoodBeingTraded.push_back(GoodBeingTraded);

    rValue2Buyer.push_back(value2Buyer);
    rValue2Seller.push_back(value2Seller);
  }
  
}
void PersonLogger::LogMoney(Person* p1,Person* p2){
  if (p1->GetBaseId() == rThePerson){
    rMoneyLog.push_back(p1->GetMoney());

  }else if (p2->GetBaseId()==rThePerson){
    rMoneyLog.push_back(p2->GetMoney());

  }

}


void PersonLogger::DumpLog(){
  int width =4;
  cout<<"Dumping Log for person"<<rThePerson<<endl;
  for (int i=0;i<rHasSnapshots.size();i++){
    int GoodNum = rGoodBeingTraded[i];
    cout<<"Snapshot # "<<i<<endl;
    cout<<"______________________________________________________________________"<<endl;
    cout<<"Person "<<rWantsSnapshots[i].personBaseId<<" wants good "<<rGoodBeingTraded[i]<<" Person "<<rHasSnapshots[i].personBaseId<<" has it"<<endl;
    cout<<"Person "<<rWantsSnapshots[i].personBaseId<<" good "<<GoodNum<<" is "<<" value to buyer "<<rValue2Buyer[i]<<" value to seller "<<rValue2Seller[i]<<" money "<<rMoneyLog[i]<<endl;
    
    PersonSnapshot *first;
    PersonSnapshot *second;
    if (rWantsSnapshots[i].personBaseId==rThePerson){ //The person being tracked is the wants person
      first = &rWantsSnapshots[i];
      second= &rHasSnapshots[i];
    } else {
      first = &rHasSnapshots[i];
      second= &rWantsSnapshots[i];
    }
    cout<<"**********************************************************************"<<endl;
    cout<<"Person "<<first->personBaseId<<endl;
    cout<<"Wants:"<<endl;
    cout<<"Good IDs  ";
    for (map<int,Good>::iterator ii = first->wants.begin();ii!=first->wants.end();ii++){
      cout<<fixed<<setw(width)<<ii->first<<" ";
    }
    cout<<endl;
    cout<<"Priority: ";
    for (map<int,Good>::iterator ii = first->wants.begin();ii!=first->wants.end();ii++){
      cout<<fixed<<setw(width)<<ii->second.GetPriority()<<" ";
    }
    cout<<endl;

    cout<<"Haves:"<<endl;
    cout<<"Good IDs  ";
    for (map<int,Good>::iterator ii = first->haves.begin();ii!=first->haves.end();ii++){
      cout<<fixed<<setw(width)<<ii->first<<" ";
    }
    cout<<endl;
    cout<<"Priority: ";
    for (map<int,Good>::iterator ii = first->haves.begin();ii!=first->haves.end();ii++){
      cout<<fixed<<setw(width)<<ii->second.GetPriority()<<" ";
    }
    cout<<endl;
  
    cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  cout<<"Person "<<second->personBaseId<<endl;
  cout<<"Wants:"<<endl;
  cout<<"Good IDs  ";
    for (map<int,Good>::iterator ii = second->wants.begin();ii!=second->wants.end();ii++){
      cout<<fixed<<setw(width)<<ii->first<<" ";
    }
    cout<<endl;
    cout<<"Priority: ";
    for (map<int,Good>::iterator ii = second->wants.begin();ii!=second->wants.end();ii++){
      cout<<fixed<<setw(width)<<ii->second.GetPriority()<<" ";
    }
    cout<<endl;

    cout<<"Haves:"<<endl;
    cout<<"Good IDs  ";
    for (map<int,Good>::iterator ii = second->haves.begin();ii!=second->haves.end();ii++){
      cout<<fixed<<setw(width)<<ii->first<<" ";
    }
    cout<<endl;
    cout<<"Priority: ";
    for (map<int,Good>::iterator ii = second->haves.begin();ii!=second->haves.end();ii++){
      cout<<fixed<<setw(width)<<ii->second.GetPriority()<<" ";
    }
    cout<<endl;
  }


}
