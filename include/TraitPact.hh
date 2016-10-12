#ifndef __TRAITPACT_HH
#define __TRAITPACT_HH







/**
   Parent class for containers to hold traits for the actors
   will be extened for each class of Economic Actor

 */


class TraitPact
{
public:
  
  TraitPact();
  virtual ~TraitPact();
  virtual void InitializeTriats()=0;



protected:

  
};
#endif /* __TRAITPACT_HH */
