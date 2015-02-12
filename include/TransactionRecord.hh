
#ifndef __TRANSACTIONRECORD_HH
#define __TRANSACTIONRECORD_HH

#include <vector>
#include <iostream>
#include <string>
using namespace std;

class TransactionRecord 
{
public:
  TransactionRecord(int g,double p,double s) : GoodNumber(g), Price(p),Supply(s) {}
  virtual ~TransactionRecord(){;}
  
  int GoodNumber;
  double Price;
  double Supply;
};



#endif /* __TRANSACTIONRECORD_HH */
