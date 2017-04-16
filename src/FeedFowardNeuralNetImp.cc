#include "FeedFowardNeuralNetImp.hh"


FeedFowardNeuralNetImp::FeedFowardNeuralNetImp(){

  rTheNetwork = new Network({1,20,1});
  //The first spot in the network will default to be the
  //want to buy food 
  unordered_map<int,string> InputNumber2Name;
  unordered_map<int,string> OutputNumber2Name;


  
}

FeedFowardNeuralNetImp::~FeedFowardNeuralNetImp(){
  delete rTheNetwork;
}

void FeedFowardNeuralNetImp::AddInput(string str){
  int theInputNumber= rTheNetwork->GetNumberOfInputNodes();
  InputNumber2Name[theInputNumber]=str;

  //The underlying network does not keep track of the names
  //only this implementaion layer does
  rTheNetwork->AddInputNode();

}


void FeedFowardNeuralNetImp::AddAction(string str){
  int theOutputNumber = rTheNetwork->GetNumberOfOutputNodes();

  OutputNumber2Name[theOutputNumber]=str;
  rTheNetwork->AddOutputNode();

}


void FeedFowardNeuralNetImp::Think(){


}

