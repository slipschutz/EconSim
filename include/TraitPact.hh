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


protected:

  
};
#endif /* __TRAITPACT_HH */
