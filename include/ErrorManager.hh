//
//
//  Manager for displaying erros and debug statements
//  
//
//
#ifndef __ERRORMANAGER_HH
#define __ERRORMANAGER_HH


#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class ErrorManager{
public:
  static void BoxPrint(string);
  static void BoxPrint(string,string);

private:
  static  vector<int> GetSpaces(string);
  static   int FindSpot(vector<int>,int);
  static  string GetRepeat(char c,int n);

};
#endif
