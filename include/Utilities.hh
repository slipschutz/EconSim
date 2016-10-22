

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
