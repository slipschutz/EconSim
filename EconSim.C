

//Generral Headers
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Person.hh"
#include "MatrixMaker.hh"
#include "Temp.hh"
#include "PersonManager.hh"
using namespace  std;

int main(int argv, char ** argc){
    
  PersonManager theManager;
  

  theManager.BuildList(4);
 
  theManager.PrintConnections();

  return 0;
}
