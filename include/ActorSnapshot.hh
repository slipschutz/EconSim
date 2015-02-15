

#ifndef __ACTORSNAPSHOT_HH
#define __ACTORSNAPSHOT_HH

#include <map>
#include "Base.hh"
#include "Good.hh"

class ActorSnapshot : public Base{
public:

  void Initialize();


  std::map <int,Good> wants;
  std::map <int,Good> haves;

  int personBaseId;

};






#endif 
