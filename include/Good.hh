
#ifndef __GOOD_HH
#define __GOOD_HH


#include "Base.hh"
#include "Settings.hh"
#include <string>
using namespace std;
class Good : public Base {
public:
  Good();
  Good(int id, int copies,int,string);
  ~Good();
  Good(const Good &aGood);//Copy constructor
  void Clear();
  Good& operator= (const Good &aGood);

  void Initialize();

  inline int GetGoodId(){return rGoodId;}
  inline void SetGoodIdAndType(int v,string t){rGoodId=v;rType=t;}

  inline void IncrementPriority(){SetPriority(rPriority+1);}
  inline void SetPriority(int v){
    if (v>Settings::MaxGoodPriority){
      rPriority=Settings::MaxGoodPriority;
    }else{
      rPriority=v;
    }
  }
  
  inline int GetPriority(){return rPriority;}

  inline double GetNormPriority(){return (Settings::MaxGoodPriority-rPriority)/(Settings::MaxGoodPriority);}

  inline int GetNumberOfCopies(){return rCopiesOfGood;}
  

  void AddCopies(int num);
  void RemoveCopies(int num);


private:
  int rGoodId;
  int rCopiesOfGood;
  int rPriority;
  string rType;

  void rUpdateManagerAdd(int change);
  void rUpdateManagerRemove(int change);
};


#endif
