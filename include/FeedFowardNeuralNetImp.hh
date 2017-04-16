
#ifndef __FEEDFOWARDNEURALNETIMP_HH
#define __FEEDFOWARDNEURALNETIMP_HH

#include "NeuralNetworkInterface.hh"
#include "Network.hh"

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>


class FeedFowardNeuralNetImp : public NeuralNetworkInterface
{
public:
  FeedFowardNeuralNetImp();
  virtual ~FeedFowardNeuralNetImp();
  
  void AddInput(string );
  void AddAction(string );
  void Think();


  
private:
  //Hold a pointer to the neural network imp
  Network* rTheNetwork;
  unordered_map<int,string> InputNumber2Name;
  unordered_map<int,string> OutputNumber2Name;
  
};



#endif /* __FEEDFOWARDNEURALNETIMP_HH */
