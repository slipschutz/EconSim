



#ifndef __EXCEPTIONS_HH
#define __EXCEPTIONS_HH

#include <exception>
#include <string>
using namespace std;

// class GenException : public exception
// {
// public:
//   virtual const char* what() 
//   {
//     return "General Exception.  This is the end.  Good Bye.";
//   }
// };

class MessageException : public exception{
public:
  MessageException(string s) : rMessage(s) {}
  
  virtual const char * what()  {
    return rMessage.c_str();
  }
  
  string rMessage;
  
  ~MessageException() throw(){};
};


#endif /* __EXCEPTIONS_HH */
