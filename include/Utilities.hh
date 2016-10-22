
#ifndef __UTILITIES_HH
#define __UTILITIES_HH

#include <iostream>
#include <string>
bool DoTest(bool test,std::string message){

  if(!test){
    std::cout<<"*******Test Fail********* "<<message<<std::endl;
    return false;
  }else{
    std::cout<<"Passed "<<message<<std::endl;
    return true;
  }

}


// //Templates
// template<typename T> void BoundAboveZero(T & val);
// template<typename T> void BoundZeroToOne(T & val);

template<typename T> void BoundAboveZero(T & val){
  if (val < 0){
    val=0;
  }
}


template<typename T> void BoundZeroToOne(T & val){
  if (val <0){
    val=0;
  }else if (val >1){
    val=1;
  }
}

#endif /* __UTILITIES_HH */
