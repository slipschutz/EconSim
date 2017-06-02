
#include "FeedFowardNeuralNetImp.hh"
#include "RandomManager.hh"
#include <iomanip>
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

NeuralNetworkInterface * FeedFowardNeuralNetImp::MutateCopy(double MutationScale){

  //First copy everything
  FeedFowardNeuralNetImp *theNewImp= new FeedFowardNeuralNetImp(*this);
  
  //now change the neural net inside the implementation
  
  vector<MatrixXd> & ref2WeightsVec=theNewImp->rTheNetwork.GetWeights();
  vector<VectorXd> & ref2BiasesVec=theNewImp->rTheNetwork.GetBiases();
  
  
  for (int i=0;i<ref2WeightsVec.size();i++){
    auto temp =MatrixXd::Random(ref2WeightsVec[i].rows(),ref2WeightsVec[i].cols())/MutationScale;

    //    cout<<ref2WeightsVec[i]<<endl;
    // cout<<"================aadsf;lfasfa========"<<endl;
    ref2WeightsVec[i]+=temp;

    //    cout<<ref2WeightsVec[i]<<endl;
    //    cout<<"================aadsf;lfasfa========"<<endl;
    //    cin.get();

    auto temp2 =VectorXd::Random(ref2BiasesVec[i].rows())/MutationScale;
    ref2BiasesVec[i]+=temp2;
  }

  theNewImp->InputReferences.clear();

  return theNewImp;
}


void FeedFowardNeuralNetImp::Train(){
  int nIn= rTheNetwork.GetNumberOfInputNodes();
  int nOut=rTheNetwork.GetNumberOfOutputNodes();
  int min =std::min(nIn,nOut);

  
  vector <pair <VectorXd,VectorXd> > data;
  //Match the demand good input with the buy that good outputs
  //
  // 0 (do nothing)
  // 1 (demand good 0) ---> 1 (buy the good)
  //   (supply good 0)
  //
  //
  //
  //  cout<<"asfdsdfasdfasfadsfs"<<endl;

  for (int i=1;i<min;i+=2){
    for (int j=0;j<100;j++){
      vector <double> tempIn(min,0.);
      vector <double> tempOut(min,0.);
      
      tempIn[i]=RandomManager::GetUniform(0.9,1);
      tempOut[i]=RandomManager::GetUniform(0.9,1);
      
      auto in =Vector2Eigen(tempIn);
      auto out=Vector2Eigen(tempOut);
      data.push_back(make_pair(in,out));
    }
  }

  //now connect the low supply to add demand for a good
  for (int i=2;i<min;i+=2){
    for (int j=0;j<100;j++){
      vector <double> tempIn(min,0.);
      vector <double> tempOut(min,0.);
      tempIn[i]=RandomManager::GetUniform(50,55);
      tempOut[i]=RandomManager::GetUniform(0.9,1);
      
      auto in =Vector2Eigen(tempIn);
      auto out=Vector2Eigen(tempOut);
      data.push_back(make_pair(in,out));
    }
  }

  
  rTheNetwork.SGD(data,1,2,2);

  ///For now it is assumed that on the input side they go
  //demand for good 0 then supply for good 0
  //demand for good 1 ....
  //
  //on the output side it goes buy good 0 add demand for good 0
  //
  // so in a world with 2 goods (don't forget the extra empty input)
  // (0,1,0,0,0) means they demand good 0


  /*
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
  
  */  
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

void FeedFowardNeuralNetImp::SetInputReferences(std::vector<double> &in){

  for (int i=0;i<in.size();i++){
    InputReferences.push_back(&in[i]);
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


VectorXd FeedFowardNeuralNetImp::Think(){

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

//   for (auto i : input ){
//     printf("%6.2lf ",i);
//   }
//   cout<<endl;
//   for (int i=0;i<result.rows();i++){
//     printf("%6.2lf ",result[i]);
//   }
//   cout<<endl;

  double max =-1000;
  int index=-99;
  for (int i=0;i<result.rows();i++){
    if (result[i] >max){
      max=result[i];
      index=i;
    }
  }

  //  cout<<index<<" "<<OutputNumber2Name[index]<<endl;
  //  cin.get();
  OutputNumber2Function[index]();

  return result;
}

