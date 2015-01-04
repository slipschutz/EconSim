
#ifndef __TRANSACTIONRECORD_HH
#define __TRANSACTIONRECORD_HH

#include <vector>
#include <iostream>
#include <string>
using namespace std;

class TransactionRecord 
{
public:
  TransactionRecord(int g,double p) : GoodNumber(g), Price(p) {}
  virtual ~TransactionRecord(){;}
  
  int GoodNumber;
  double Price;
  
};



#endif /* __TRANSACTIONRECORD_HH */
