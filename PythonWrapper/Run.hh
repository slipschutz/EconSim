#include "Utilities.hh"
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

  // //  DataLogger::Get()->LogMarketState(MarketManager::Get(), GoodManager::Get());
  // MarketManager::Get()->ClearMarket();
  // Calendar::DayNumber++;



}

void Run(){
  killLoop=false;

  try{
    EconomicActorManager *  theManager = new EconomicActorManager();
    
    
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

    }

    
 
    


    //DataLogger::Get()->LogEndingMoneyDistribution(theManager->GetList());

    delete theManager;

    

    
  } catch (char e){// catch (exception &e){
    cout<<"*******************************************************"<<endl;
    cout<<"Fatal Exception Must exit"<<endl;
    //    cout<<e.what()<<endl;
    cout<<"*******************************************************"<<endl;
  }
  


}



void MoreTests(){

  EconomicActorManager * man =new EconomicActorManager();
  double totalMoney=0;
  Person *lucky;
  Settings::CanStarveToDeath=false;
  for (int i=0;i<10;i++){
    Person *aPerson1 = new Person(man);
    aPerson1->Initialize();
    man->MakeActor(aPerson1);
    totalMoney+=aPerson1->GetMoney();
    lucky=aPerson1;
  }

  for (int i=0;i<2;i++){
    totalMoney+=10000;
    Manufacturer * m = new Manufacturer(10000,man,lucky,0);
    m->SetActorLogger(new ActorLogger(m->GetBaseId()));
    man->MakeActor(m);
  }
  cout<<setprecision(20);


  for (int i=0;i<1000;i++){
    man->DoAStep();
    man->GetMarketManager()->ClearMarket();
    //    Calendar::DayNumber++;
  }

  double endingMoney=0;
  for (auto i : *man->GetList()){
    endingMoney+=i.second->GetMoney();
  }

  DoTest(abs(endingMoney-totalMoney)<0.0000001,"Money should be conserved");
  delete man;
  Settings::CanStarveToDeath=true;
}



void NetworkTests(){

  //a defaults to 1--30--1 network structure
  Network a;

  DoTest(a.GetBiases().size()==2,"The Network biases should be size 2");
  DoTest(a.GetWeights().size()==2,"The Network weights should size 2");

  
  DoTest(a.GetWeights()[0].cols()==1,"The Network should defualt to 1 input argument");
  //Add an input argument
  a.AddInputNode();
  DoTest(a.GetWeights()[0].cols()==2,"The Network should now have 2 input arguments");

  DoTest(a.GetWeights()[1].rows()==1,"The Network should have one output node seen in the weights");
  DoTest(a.GetBiases()[1].rows()==1,"The Network should have one output node seen in the biases");

  a.AddOutputNode();
  DoTest(a.GetWeights()[1].rows()==2,"The Network should have two output node seen in the weights");
  DoTest(a.GetBiases()[1].rows()==2,"The Network should have two output node seen in the biases");


  Network b({1,1,1});
  b.MakeEveryThing1();

  double input=1;
  vector<double> in = {input};
  auto ans =b.FeedFoward(in);

  input=input+1;
  input = 1/(1+exp(-input));
  input=input+1;
  input = 1/(1+exp(-input));
  DoTest(input==ans[0],"The result for this network should work for this simple example");

  
  
}

void UnitTests(){
  MoreTests();
  NetworkTests();
  EconomicActorManager * man =new EconomicActorManager();


  //Basic People Tests
  Person aPerson1(man);
  man->MakeActor(&aPerson1);


  int totalSupply = (*aPerson1.GetSupplies())[0].GetNumberOfCopies();
  DoTest(GoodManager::Get()->supply[0]==totalSupply,"Total Supply should be the supply of person 1");

  Person aPerson2(man);
  man->MakeActor(&aPerson2);


  totalSupply += (*aPerson2.GetSupplies())[0].GetNumberOfCopies();
  DoTest(GoodManager::Get()->supply[0]==totalSupply,"Total Supply should be the supply of person 1 and 2");

  Manufacturer *aManufac= new Manufacturer(1000,man,&aPerson1,0);

  man->MakeActor(aManufac);
  DoTest(aManufac->GetMoney()==1000,"Manufacter should start with 1000 dollars");

  string aString="Company owner not the right person";

  DoTest(aManufac->GetTheOwner()==&aPerson1,aString);

  aManufac->AddEmployee(&aPerson2,100);

  DoTest(aManufac->GetNumberOfEmployees()==1,"Company Should Have 1 Employee");


  int totalAmtForManu=25;
  totalSupply += totalAmtForManu;

  aManufac->SetGoodPriority(0,50);//Set good 0 to proity 50 (should mean price=50)
  aManufac->AddSupply(0,totalAmtForManu);


  DoTest(GoodManager::Get()->supply[0]==totalSupply,"Total Supply of good 0 equals person1+person2+25");

  
  Person aPerson3(man);
  man->MakeActor(&aPerson3); 
  totalSupply += (*aPerson3.GetSupplies())[0].GetNumberOfCopies();

  DoTest(GoodManager::Get()->supply[0]==totalSupply,"Total Supply of good 0 equals person1+person2+25+person3");

  aPerson3.SetRestlessnes    (0);
  aPerson3.SetGluttoness     (0);
  aPerson3.SetInvestmentLevel(0);
  aPerson3.AddDemand(0,10);

  DoTest(10==GoodManager::Get()->demand[0],"Total demand should be 10");


  aPerson3.BeginningOfStep();


  man->GetMarketManager()->PlaceSellOrder(0,aManufac->GetBaseId(),
				       totalAmtForManu,aManufac->GetGoodPriority(0)); 


  auto goods4SaleBefore =(man->GetMarketManager()->GetCurrentGoodsForSale());


  DoTest(int(goods4SaleBefore.size())==Settings::MaxGoodNumber,"The goods for sale vector in market manager should be of length Settings::MaxGoodNumber");

  DoTest(goods4SaleBefore.at(0)==25,"There should be 25 of good 0 still left for sale before any sales have happened");
				   
  double preStepMoney=aPerson3.GetMoney();
  aPerson3.DoStep();

  auto goods4Sale =(man->GetMarketManager()->GetCurrentGoodsForSale());
  DoTest(goods4Sale.at(0)==15,"There should be 25-10 of good 0 still left for sale");
  



  DoTest(aPerson3.GetMoney()==preStepMoney-10*50,"Person3 should have lost money equal to the transaction");

  DoTest(totalSupply==GoodManager::Get()->supply[0],"Total supply should be the same");

  DoTest(0==GoodManager::Get()->demand[0],"Total demand should be 0");

  

  aPerson2.SetRestlessnes    (0);
  aPerson2.SetGluttoness     (0);
  aPerson2.SetInvestmentLevel(0);
  aPerson2.AddDemand(0,50);

  double preStepPerson2=aPerson2.GetMoney();
  aPerson2.DoStep();
  //There should be 15 units of 0 left on the market
  DoTest(totalSupply==GoodManager::Get()->supply[0],"Total supply should be the same");
  DoTest(35==GoodManager::Get()->demand[0],"Total demand should be 35");

  DoTest(preStepPerson2-15*50==aPerson2.GetMoney(),"Person2 should have lost 15*50 dollars");


  DoTest(50*25+1000 == aManufac->GetMoney(),"Manufacturer should now have the money from person2,3");

  aManufac->EndOfStep(); 

  DoTest(preStepPerson2-15*50 + 100==aPerson2.GetMoney(),"Person2 should have now gotten paid 100");



  Person aPerson4(man);
  aPerson4.Initialize();
  DoTest(aPerson4.GetHasJob()==false,"Person should not have a job yet");
  man->GetMarketManager()->PlaceJobPosting(10,aManufac->GetBaseId());
  JobInfo jInfo= man->GetMarketManager()->GetBestJob();
  DoTest(jInfo.EmployerID!=-1,"There should be a job available on the market");
  aPerson4.DoStep();
  DoTest(aPerson4.GetHasJob()==true,"Person should Now have a job");
  DoTest(aPerson4.GetEmployer()->GetBaseId()==aManufac->GetBaseId(),"The company* inside of person should be correct");

  double moneyPerson4Orig=aPerson4.GetMoney();
  double moneyManuOrig =aManufac->GetMoney();
  aManufac->EndOfStep();
  DoTest(moneyPerson4Orig+10 ==aPerson4.GetMoney(),"Person4 should have been paid 10");

  DoTest(moneyManuOrig-10-100 == aManufac->GetMoney(),"manufac should have give 10 money away to person4 and a 100 to person2");

  
  DoTest((*aManufac->GetSupplies())[0].GetNumberOfCopies() == 0,"The Manufacturer should have no supply left");

}
