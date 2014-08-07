
#ifndef __GOOD_HH
#define __GOOD_HH


#include "Base.hh"

class Good : public Base {
public:
  Good();
  ~Good();
  
  void Initialize();

  inline int GetGoodId(){return rGoodId;}
  inline void SetGoodId(int v){rGoodId=v;}

  inline void IncrementPriority(){rPriority++;}
  inline void SetPriority(int v){rPriority=v;}
  
  inline int GetPriority(){return rPriority;}
  
private:
  int rGoodId;
  
  int rPriority;


};


#endif
