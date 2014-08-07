
#include "PersonLogger.hh"
#include "Person.hh"
#include "PersonSnapshot.hh"
#include "Good.hh"

PersonLogger::PersonLogger(): rThePerson(-1){

  Initialize();
}

PersonLogger::~PersonLogger(){

}


void PersonLogger::Initialize(){
  rWantsSnapshots.clear();
  rHasSnapshots.clear();
  rGoodBeingTraded.clear();
}

void PersonLogger::CheckAndLogTransaction(Person* pWants,Person* pHas,int GoodBeingTraded,int probNum){

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
    rProbForGood.push_back(probNum);
  }
  
}

void PersonLogger::DumpLog(){
  cout<<"Dumping Log for person"<<rThePerson<<endl;
  for (int i=0;i<rHasSnapshots.size();i++){
    int GoodNum = rGoodBeingTraded[i];
    cout<<"Snapshot # "<<i<<endl;
    cout<<"______________________________"<<endl;
    cout<<"Person "<<rWantsSnapshots[i].personBaseId<<" wants good "<<rGoodBeingTraded[i]<<" Person "<<rHasSnapshots[i].personBaseId<<" has it"<<endl;
    cout<<"The Piority for person "<<rWantsSnapshots[i].personBaseId<<" for good "<<GoodNum<<" is "<<rWantsSnapshots[i].wants[GoodNum].GetPriority()<<" the prob num was "<<rProbForGood[i]<<endl;

    cout<<"Person "<<rWantsSnapshots[i].personBaseId<<endl;
    cout<<"Wants:"<<endl;
    cout<<"    ";
    for (map<int,Good>::iterator ii = rWantsSnapshots[i].wants.begin();ii!=rWantsSnapshots[i].wants.end();ii++){
      cout<<ii->first<<" ";
    }
    cout<<endl;

    cout<<"Haves:"<<endl;
    cout<<"    ";
    for (map<int,Good>::iterator ii = rWantsSnapshots[i].haves.begin();ii!=rWantsSnapshots[i].haves.end();ii++){
      cout<<ii->first<<" ";
    }
    cout<<endl;


    cout<<"Person "<<rHasSnapshots[i].personBaseId<<endl;
    cout<<"Wants:"<<endl;
    cout<<"    ";
    for (map<int,Good>::iterator ii = rHasSnapshots[i].wants.begin();ii!=rHasSnapshots[i].wants.end();ii++){
      cout<<ii->first<<" ";
    }
    cout<<endl;

    cout<<"Haves:"<<endl;
    cout<<"    ";
    for (map<int,Good>::iterator ii = rHasSnapshots[i].haves.begin();ii!=rHasSnapshots[i].haves.end();ii++){
      cout<<ii->first<<" ";
    }
    cout<<endl;

  }

}
