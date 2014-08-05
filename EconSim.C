

//Generral Headers
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Person.hh"
#include "MatrixMaker.hh"
#include "Temp.hh"
#include "PersonManager.hh"
#include "RandomManager.hh"
using namespace  std;

int main(int argv, char ** argc){
    
  PersonManager theManager;
  

  theManager.BuildList(100);
 
  theManager.PrintConnections();
  
  theManager.PrintHavesWants(1);
  for (int i=0;i<100;i++){
    theManager.MakeTransactions();
  }
  theManager.PrintHavesWants(1);

  return 0;
}
