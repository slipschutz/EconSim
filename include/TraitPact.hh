#ifndef __TRAITPACT_HH
#define __TRAITPACT_HH

#include <vector>
#include <boost/any.hpp>

/**
   Abstract parent class for containers to hold traits for the actors
   will be extened for each class of Economic Actor
 */


// class Trait{
// public:

//   virtual void * Get()=0;


// };


// template <typename T>
// class TraitImpl : public Trait{

// private:
//   T val;

// };




class TraitPact
{
public:
  
  TraitPact();
  virtual ~TraitPact();
  virtual void InitializeTraits()=0;



  template <typename T> void AddTrait(T val){
    theTraits.push_back(val);
  }

  template <typename T> void GetTrait(int index,T & val){
    val=boost::any_cast<T>(theTraits.at(index));
  }

  boost::any GetTrait(int index){
    return theTraits.at(index);
  }



  int GetSize(){return theTraits.size();}

private:
  //a vector of boost any objects will hold the traits
  std::vector <boost::any> theTraits;  
};

#endif /* __TRAITPACT_HH */
