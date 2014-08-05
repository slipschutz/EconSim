
#include "RandomManager.hh"
#include <stdlib.h>
#include <time.h>
#include <iostream>
bool RandomManager::Seeded=false;

RandomManager::RandomManager(){
 
  
}


int RandomManager::GetRand(int n){
  if (!Seeded){
    srand(time(NULL));
    Seeded=true;
  }
  return rand() % n;
}

