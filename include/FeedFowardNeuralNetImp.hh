
#ifndef __FEEDFOWARDNEURALNETIMP_HH
#define __FEEDFOWARDNEURALNETIMP_HH

#include "NeuralNetworkInterface.hh"
#include "Network.hh"

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>



class FeedFowardNeuralNetImp : public NeuralNetworkInterface
{
public:
  FeedFowardNeuralNetImp();
  virtual ~FeedFowardNeuralNetImp();
  
  void AddInput(string,double * val_add);
  void AddAction(string,std::function<void()> );

  void SetInputReferences(std::vector<double> &);

  VectorXd Think();

  void Print();
  void Train();

  NeuralNetworkInterface * MutateCopy(double MutationScale=10);
  



private:
  Network rTheNetwork;  

  unordered_map<int,string> InputNumber2Name;
  unordered_map<int,string> OutputNumber2Name;
  unordered_map<int,std::function<void() > > OutputNumber2Function;
  vector<double*> InputReferences;
};



#endif /* __FEEDFOWARDNEURALNETIMP_HH */
