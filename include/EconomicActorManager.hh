#ifndef __PERSONMANAGER_HH
#define __PERSONMANAGER_HH

//
// Person Manager class. 
//
//


#include "Base.hh"
#include <vector>
#include <map>
#include <unordered_map>

#include "Settings.hh"
using namespace std;


//Foward Declarations
class EconomicActor;
class Manufacturer;
class Person;
class Company;
class DataLogger;
class MarketManager;

class EconomicActorManager : public Base {
public:

  /**Constructor
   */
  EconomicActorManager();

  /**Destructor
   */
  ~EconomicActorManager();

  /**Main function to build the people/companies that make up the simualtion
   */
  void BuildCompleteNetwork(int NumberOfPeople);

  /**Mandatory methods from Base class
   */
  void Initialize();


  /**Old method to print the supplies and 
     demands for all the actors*/
  void PrintHavesWants();
  /**Prints supplies and demands for an actor with given
     ID*/
  void PrintHavesWants(int);


  /**Moves the simulation foward 1 step.  IE the
     main bit of code for the sim*/
  void DoAStep();


  /**Adds an actor the list of actors to be deleted at the end of the 
     time step.  The Base Id is added to the vector rToBeKilled
  */
  void MarkForDeath(EconomicActor*);

  /**Function to add an actor to the main list of actors
   */
  void MakeActor(EconomicActor*);


  /**Returns a raw pointer to the hash that contains the all the actor IDs
     and pointers*/
  unordered_map <int,EconomicActor*> * GetList(){return &rTheListOfActors;}


  /**Obvious
   */
  Person * FindPerson(int Id);
  /**Obvious
   */
  Company * FindCompany(int Id);

  /**Builds small test network
   */
  void BuildTestNetwork();

  /**Old printing function
   */
  void PrintAllInfo();
  
  /**Obvious
   */
  int GetNumberOfPeople(){return Settings::NumberOfPeople-rNumberOfPeopleDeaths;}

  DataLogger * GetDataLogger(){return rTheDataLogger;}
  MarketManager * GetMarketManager(){return rTheMarketManager;}


  vector<double> GetPriceData();

  vector<int> GetPopulation();
  vector<int> GetNumManufacturers();
  vector<int> GetDemands(int n);
  vector<int> GetSupplies(int n);


private:
  int rNumberOfPeople;
  
  int rNumCurrentCompanies;
  std::unordered_map <int,EconomicActor*> rTheListOfActors;
  vector <int> rTheIds;///<Vector that is used to determine the order that the Actors act in
  vector <int> rToBeKilled;


  DataLogger *rTheDataLogger;
  MarketManager *rTheMarketManager; 



  int rNumberOfPeopleDeaths;
  int rNumberOfPeopleBirths;
  int rNumberOfCompanyBirths;
  int rNumberOfCompanyDeaths;
  int rNumberOfManufacturers;
  int rNumberOfBirths;
};


#endif
