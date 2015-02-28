
#ifndef __DEATH_HH
#define __DEATH_HH

#include <map>
#include <iostream>
#include <string>
using namespace std;

class Death 
{
public:

  virtual ~Death(){}

  static Death* theManager;
  static Death* Get();

  map<int,int> TheDead;
  void AddDead(int id,int day);

  void FindDead(int id);

private:
  Death();
};



#endif /* __DEATH_HH */
