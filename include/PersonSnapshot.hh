

#ifndef __PERSONSNAPSHOT_HH
#define __PERSONSNAPSHOT_HH

#include <map>
#include "Base.hh"
#include "Good.hh"

class PersonSnapshot : public Base{
public:

  void Initialize();


  std::map <int,Good> wants;
  std::map <int,Good> haves;

  int personBaseId;

};






#endif 
