
#ifndef __GOODMANAGER_HH
#define __GOODMANAGER_HH

#include <vector>
#include <iostream>
#include <string>
#include <map>
using namespace std;

class Good;
class Person;

class GoodManager 
{
public:

  virtual ~GoodManager();

  static GoodManager *theManager;

  static GoodManager * Get();

  map<int,int> supply;
  map<int,int> demand;

  double GetWorthToBuyer(Person * thePerson,int GoodID);
  double GetWorthToSeller(Person * thePerson,int GoodID);
  
  Good MakeWant(int id, int copies);
  Good MakeHave(int id, int copies);

  Good ReplaceHave(int id, int copies);
 
private:
  

  GoodManager();  
};



#endif /* __GOODMANAGER_HH */
