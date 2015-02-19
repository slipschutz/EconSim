#include "Good.hh"
#include "GoodManager.hh"
using namespace std;

Good::Good(){
  rGoodId=-1;
  rPriority=0;
  rCopiesOfGood=0;
  rType="!!!__";

}

Good::Good(int id, int copies,int prior,string t){// : rGoodId(id), rCopiesOfGood(copies),rPriority(prior),rType(t){

  rGoodId=id;
  rCopiesOfGood=copies;
  rPriority=prior;
  rType=t;
  rUpdateManagerAdd(copies);
}
Good::~Good(){
  //Update the manager that these goods have been destroyed
  //  rUpdateManagerRemove(rCopiesOfGood);

}
void Good::Clear(){


  rUpdateManagerRemove(rCopiesOfGood);
}

// Good::Good(const Good &aGood){//Copy constructor
//   this->rGoodId = aGood.rGoodId;
//   this->rCopiesOfGood=aGood.rCopiesOfGood;
//   this->rPriority=aGood.rPriority;
//   this->rType=aGood.rType;

// }

// Good& Good::operator= (const Good &aGood){
//   this->rGoodId = aGood.rGoodId;
//   this->rCopiesOfGood=aGood.rCopiesOfGood;
//   this->rPriority=aGood.rPriority;
//   this->rType=aGood.rType;
  
//   return *this;
// }


void Good::Initialize(){
  
  rGoodId=-1;
  rPriority=0;
  rCopiesOfGood=0;
  rType="!!!__";
}


void Good::AddCopies(int num){
  if (rGoodId==-1 ){
    cout<<"<Good::AddCopies> can't AddCopies without a GoodId set"<<endl;
    throw 1;
  }
  rCopiesOfGood+=num;
  rUpdateManagerAdd(num);
}


void Good::RemoveCopies(int num){
  if (rGoodId==-1){
    cout<<"<Good::RemoveCopies> Can't remove Copies without a GoodId set"<<endl;
    throw 1;
  }
  if (rCopiesOfGood < num ){
    cout<<"Error <Good::RemoveCopies>: Trying to remove "<<num<<" copies but there are only "<<rCopiesOfGood<<endl;
    throw 1;
  } else{
    rCopiesOfGood-=num;
  }
  rUpdateManagerRemove(num);
}


void Good::rUpdateManagerAdd(int change){
  if (rType == "demand"){
    GoodManager::Get()->AddDemand(rGoodId,change);
  }else if (rType == "supply"){
    GoodManager::Get()->AddSupply(rGoodId,change);
  }else {
    cout<<"<Good::rUpdateManagerAdd> Invaid type "<<rType<<endl;
    throw 1;
  }
}

void Good::rUpdateManagerRemove(int change){
  if (rType == "demand"){
    GoodManager::Get()->RemoveDemand(rGoodId,change);
  }else if (rType == "supply"){
    GoodManager::Get()->RemoveSupply(rGoodId,change);
  }else {
    cout<<"<Good::rUpdateManagerRemove> Invaid type "<<rType<<endl;
    throw 1;
  }
}
