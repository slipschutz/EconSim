

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
  

  theManager.BuildList(10000);
  theManager.SetPersonToLog(66);
  theManager.PrintConnections();
  

  for (int i=0;i<500;i++){
    theManager.MakeTransactions();
  }
  
  theManager.GetLogger()->DumpLog();

  
  return 0;
}
