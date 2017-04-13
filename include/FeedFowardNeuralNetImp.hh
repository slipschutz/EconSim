
#ifndef __FEEDFOWARDNEURALNETIMP_HH
#define __FEEDFOWARDNEURALNETIMP_HH

#include "NeuralNetworkInterface.hh"
#include "Network.hh"

#include <vector>
#include <memory>

class FeedFowardNeuralNetImp : public NeuralNetworkInterface
{
public:
  FeedFowardNeuralNetImp();
  virtual ~FeedFowardNeuralNetImp();
  
  void AddInput();
  void AddAction();
  void Think();

private:
  //Hold a pointer to the neural network imp
  Network* rTheNetwork;
};




#endif /* __FEEDFOWARDNEURALNETIMP_HH */
