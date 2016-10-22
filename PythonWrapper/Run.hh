
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


bool killLoop;
void my_handler(int s){
  printf("Caught signal %d\n",s);
  killLoop=true;
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

      if(i > 500){
	if (i % 100 ==0){
	  if (Settings::FoodProductionPerWorker==2){
	    Settings::FoodProductionPerWorker=10;
	  }else {
	    Settings::FoodProductionPerWorker=2;
	  }
	}

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
      if (i==Settings::NumberOfSteps-1){
	// cout<<"\n\n\n";
	// GoodManager::Get()->Dump();
	
	// cin.get();
      }
      DataLogger::Get()->LogMarketState(MarketManager::Get(), GoodManager::Get());

      MarketManager::Get()->ClearMarket();
      Calendar::DayNumber++;

    }


 
    

    //theManager.PrintMoney();
    // theManager.PrintHavesWants();
    DataLogger::Get()->LogEndingMoneyDistribution(theManager->GetList());

    delete theManager;
    //    GoodManager::Get()->Dump();
    


    // if (DataLogger::Get() != NULL){
    //   delete DataLogger::Get();
    // }
  } catch (exception &e){
    cout<<"*******************************************************"<<endl;
    cout<<"Fatal Exception Must exit"<<endl;
    cout<<e.what()<<endl;
    cout<<"*******************************************************"<<endl;
  }
  


}
