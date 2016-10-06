
#include "RandomManager.hh"

 
#include <chrono>
#include <iostream>
bool RandomManager::pSeeded=false;
std::default_random_engine* RandomManager::pTheGenerator = NULL;

unsigned RandomManager::rTheSeed=0;

RandomManager::RandomManager(){
 
  
}


int RandomManager::GetRand(int n){
  if (!pSeeded){
    // construct a trivial random generator engine from a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();//2029165674;
    rTheSeed=seed;
    std::cout<<"SEED IS "<<seed<<std::endl;
    pTheGenerator = new std::default_random_engine(seed);

    pSeeded=true;
  }

  std::uniform_int_distribution<int> distribution(0,n-1);
  

  //  std::cout<<"n is "<<n<<" and number is "<< distribution(*pTheGenerator)<<std::endl;
  //  int t; std::cin>>t;
  return distribution(*pTheGenerator);

}

unsigned RandomManager::GetSeed(){
  return rTheSeed;
}

double RandomManager::GetGaus(double mean,double sigma){
    if (!pSeeded){
    // construct a trivial random generator engine from a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rTheSeed=seed;
    pTheGenerator = new std::default_random_engine(seed);

    pSeeded=true;
  }

    std::normal_distribution<double> distribution(mean,sigma);
    return distribution(*pTheGenerator);
}

double RandomManager::GetUniform(double low,double high){
  if (!pSeeded){
    // construct a trivial random generator engine from a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rTheSeed=seed;
    pTheGenerator = new std::default_random_engine(seed);

    pSeeded=true;
  }

  std::uniform_real_distribution<double> distribution(low,high);
  return distribution(*pTheGenerator);


}

