#ifndef __UNIQUE_HH
#define __UNIQUE_HH
#include "Base.hh"

class Unique : public Base {
public:
  inline int GetUniqueId(){return rUniqueId;}
  inline void SetUniqueId(int d){rUniqueId=d;}

protected:
  static int GetNextId();
private:
  static int rNext_Id;
  int rUniqueId;

};

#endif
