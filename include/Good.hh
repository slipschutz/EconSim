
#ifndef __GOOD_HH
#define __GOOD_HH


#include "Settings.hh"
#include <string>
#include <type_traits>
#include <istream>


enum class GoodType
{ Demand,Supply,None};



using namespace std;
class Good  {
public:
  Good();
  Good(int id, int copies,int,GoodType);
  ~Good();

  void Clear();

  inline int GetGoodId(){return rGoodId;}

  inline void IncrementPriority(){SetPriority(rPriority+1);}

  inline void SetPriority(int v){
    if (v>Settings::MaxGoodPriority){
      rPriority=Settings::MaxGoodPriority;
    }else{
      rPriority=v;
    }
  }
  
  inline int GetPriority(){return rPriority;}

  inline int GetNumberOfCopies(){return rCopiesOfGood;}

  void AddCopies(int num);
  void RemoveCopies(int num);

private:
  int rGoodId;
  int rCopiesOfGood;
  int rPriority;
  GoodType rType;

  void rUpdateManagerAdd(int change);
  void rUpdateManagerRemove(int change);
};


#endif
