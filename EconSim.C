

//Generral Headers
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Person.hh"
using namespace  std;

int main(int argv, char ** argc){
  
  srand(0);

  
  vector <Person*> theList(10);
  for (int i=0;i<10;i++){
    theList[i]=new Person();
  }

  for (int i=0;i<theList.size();i++){
    int n= rand() %10 + 1;
    if (n != i ){
      theList[i]->MakeConnection(theList[n]);
    }
    
  }
  for (int i=0;i<10;i++){
    theList[i]->DumpConnections();
  }

 
  return 0;
}
