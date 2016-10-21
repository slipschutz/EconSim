#ifndef __TRAITPACT_HH
#define __TRAITPACT_HH







/**
   Abstract parent class for containers to hold traits for the actors
   will be extened for each class of Economic Actor
 */



class TraitPact
{
public:
  
  TraitPact();
  virtual ~TraitPact();
  virtual void InitializeTraits()=0;

  void BoundAboveZero(int &val);
  void BoundAboveZero(float&val);
  template<typename T> void BoundAboveZero(T & val);
  
  void BoundZeroToOne(float &val);
    

protected:

  
};


template<typename T> void TraitPact::BoundAboveZero(T & val){

}


#endif /* __TRAITPACT_HH */
