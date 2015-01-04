#include "Good.hh"


Good::Good(){

  Initialize();
}

Good::Good(int id, int copies) : rGoodId(id), rCopiesOfGood(copies)
{}
Good::~Good(){


}


void Good::Initialize(){
  
  rGoodId=-1;
  rPriority=0;
  rCopiesOfGood=1;
}



