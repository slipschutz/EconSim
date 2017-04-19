
#include "FeedFowardNeuralNetImp.hh"
using std::cout;
using std::end;

FeedFowardNeuralNetImp::FeedFowardNeuralNetImp(){

  //The first input and output have to be there to construct
  //the underlying network, so they will be initialized to do
  //nothing
  InputNumber2Name[0]="Nothing";
  OutputNumber2Name[0]="Nothing";
  OutputNumber2Function[0]=[](){};//Set to an empty lambda

}

void FeedFowardNeuralNetImp::Train(){
  int n=-1;
  for (auto i : InputNumber2Name){
    if (i.second =="FoodDemand"){
      n=i.first;
      break;
    }
  }
  int n2=-1;
  for (auto i : OutputNumber2Name){
    if (i.second =="BuyFood"){
      n2=i.first;
      break;
    }
  }
  
  int max= std::max(n,n2);
  vector<double> temp;
  vector<double> temp2;
  max=3;
  for (int i=0;i<max;i++){
    if (i==n){
      temp.push_back(1);
    }else{
       temp.push_back(0);
    }
    if (i<2){
      if(i==n2){
	temp2.push_back(1);
      }else{
	temp2.push_back(0);
      }
    }
  }
  auto in =Vector2Eigen(temp);
  auto out=Vector2Eigen(temp2);
  vector <pair <VectorXd,VectorXd> > data;
  
  for (int i=0;i<50;i++){
    data.push_back(make_pair(in,out));
  }
  
  rTheNetwork.SGD(data,1,2,0.5);
  
  
}

FeedFowardNeuralNetImp::~FeedFowardNeuralNetImp(){
  //  delete rTheNetwork;
}

void FeedFowardNeuralNetImp::Print(){
  for (auto i : InputNumber2Name){
    cout<<i.second<<endl;
  }
  for (auto i : OutputNumber2Name){
    cout<<i.second<<endl;
  }
  

}

void FeedFowardNeuralNetImp::AddInput(string str,double *val_add){
  int theInputNumber= rTheNetwork.GetNumberOfInputNodes();
  InputNumber2Name[theInputNumber]=str;

  //The underlying network does not keep track of the names
  //only this implementaion layer does
  rTheNetwork.AddInputNode();

  InputReferences.push_back(val_add);
}


void FeedFowardNeuralNetImp::AddAction(string str,std::function<void()> func){
  int theOutputNumber = rTheNetwork.GetNumberOfOutputNodes();

  OutputNumber2Name[theOutputNumber]=str;
  rTheNetwork.AddOutputNode();

  OutputNumber2Function[theOutputNumber]=func;

  
      
}


void FeedFowardNeuralNetImp::Think(){

  vector <double> input;

  ///this first spot in the network at the moment is
  //the reserved do nothing spot
  //so this push back is needed to make the input
  //vector the right size
  input.push_back(0);

  
  for (auto & i : InputReferences){
    input.push_back(*i);
  }

  
  auto result =rTheNetwork.FeedFoward(input);

  double max =-1000;
  int index=-99;
  for (int i=0;i<result.rows();i++){
    if (result[i] >max){
      max=result[i];
      index=i;
    }
  }

  OutputNumber2Function[index]();
}

