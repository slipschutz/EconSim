
#ifndef __GOOD_HH
#define __GOOD_HH


#include "Base.hh"
#include "Settings.hh"
class Good : public Base {
public:
  Good();
  Good(int id, int copies);
  ~Good();
  
  void Initialize();

  inline int GetGoodId(){return rGoodId;}
  inline void SetGoodId(int v){rGoodId=v;}

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
  inline void SetNumberOfCopies(int v){rCopiesOfGood=v;}
  


private:
  int rGoodId;
  
  int rPriority;
  
  int rCopiesOfGood;

};


#endif
