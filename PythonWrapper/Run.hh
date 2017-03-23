
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


bool killLoop;
void my_handler(int s){
  printf("Caught signal %d\n",s);
  killLoop=true;
}


void DoEndOfDay(){

  DataLogger::Get()->LogMarketState(MarketManager::Get(), GoodManager::Get());
  MarketManager::Get()->ClearMarket();
  Calendar::DayNumber++;



}

void Run(){
  killLoop=false;

  try{
    EconomicActorManager *  theManager = new EconomicActorManager();
    
    DataLogger::Get();
    
    theManager->BuildCompleteNetwork(Settings::NumberOfPeople);
  
    cout<<"BUILT NETWORK"<<endl;

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    
    for (int i=0;i<Settings::NumberOfSteps;i++){
      sigaction(SIGINT, &sigIntHandler, NULL);    
      if (killLoop){
	printf("killing loop \n");
	break;
      }
      
      if ( i >1000){
	// if (i%500==0){
	//   if (Settings::FoodProductionPerWorker==0){
	//     cout<<"----> Food now at 12 "<<endl;
	//     Settings::FoodProductionPerWorker=12;
	//   }else{
	//     Settings::FoodProductionPerWorker=0;
	//     cout<<"----> Food now at 0 "<<endl;
	//   }

	// }
      }
      

      theManager->DoAStep();
      // MarketManager::Get()->Dump();
      // cin.get();
      if (i%10==0){
	cout<<"ON "<<i<<endl;
	// cout<<"VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV"<<endl;
	// GoodManager::Get()->Dump();
	// MarketManager::Get()->Dump();
	// cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl;
	// cin.get();

	
      }
      DoEndOfDay();
    }

    
 
    


    //DataLogger::Get()->LogEndingMoneyDistribution(theManager->GetList());

    delete theManager;

    

    
  } catch (exception &e){
    cout<<"*******************************************************"<<endl;
    cout<<"Fatal Exception Must exit"<<endl;
    cout<<e.what()<<endl;
    cout<<"*******************************************************"<<endl;
  }
  


}
