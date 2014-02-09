
#include "Unique.hh"

int Unique::rNext_Id=0;

int Unique::GetNextId(){
  int d = Unique::rNext_Id++;
  return d;
}

