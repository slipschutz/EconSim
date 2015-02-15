#ifndef __BASE_HH
#define __BASE_HH
#include <iostream>

/**
//  
//  Abstract Base class for this program
//
*/

class Base {
public:
  Base();
  Base( const Base& other ); //Copy Constructor
  virtual ~Base();

  
  virtual void Initialize()=0; 
  
  inline int GetBaseId(){return rBaseId;}
  inline void SetBaseId(int d){rBaseId=d;}
  
  static int GetNumDeletes(){return Base::rNumDeletes;}
  static int GetCurrentId(){return Base::rNextBase_Id;}

  void PrintLine(char c,int l){
    for (int i=0;i<l;++i){
      std::cout<<c;
    }std::cout<<std::endl;
  }
private:
  int rBaseId;


  static int rNextBase_Id;
  static int rGetNextBaseId();
  static int rNumDeletes;
  
};





#endif
