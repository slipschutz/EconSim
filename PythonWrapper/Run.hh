



void Run(){

  try{
    EconomicActorManager *  theManager = new EconomicActorManager();
    
    DataLogger::Get();
    
    theManager->BuildCompleteNetwork(Settings::NumberOfPeople);
  
    cout<<"BUILT NETWORK"<<endl;

  
    for (int i=0;i<Settings::NumberOfSteps;i++){
    
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
