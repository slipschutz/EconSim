
#include "RandomManager.hh"

 
#include <chrono>
#include <iostream>
bool RandomManager::pSeeded=false;
std::default_random_engine* RandomManager::pTheGenerator = NULL;

RandomManager::RandomManager(){
 
  
}


int RandomManager::GetRand(int n){
  if (!pSeeded){
    // construct a trivial random generator engine from a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    pTheGenerator = new std::default_random_engine(seed);

    pSeeded=true;
  }

  std::uniform_int_distribution<int> distribution(0,n-1);
  

  //  std::cout<<"n is "<<n<<" and number is "<< distribution(*pTheGenerator)<<std::endl;
  //  int t; std::cin>>t;
  return distribution(*pTheGenerator);

}

double RandomManager::GetGaus(double mean,double sigma){
    if (!pSeeded){
    // construct a trivial random generator engine from a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    pTheGenerator = new std::default_random_engine(seed);

    pSeeded=true;
  }

    std::normal_distribution<double> distribution(mean,sigma);
    return distribution(*pTheGenerator);
}

