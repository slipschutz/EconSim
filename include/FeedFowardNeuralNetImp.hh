
#ifndef __FEEDFOWARDNEURALNETIMP_HH
#define __FEEDFOWARDNEURALNETIMP_HH

#include "NeuralNetworkInterface.hh"
#include "Network.hh"

#include <vector>

class FeedFowardNeuralNetImp : public NeuralNetworkInterface
{
public:
  FeedFowardNeuralNetImp();
  virtual ~FeedFowardNeuralNetImp();
  
  void AddInput();
  void AddAction();
  void Think();

private:

Network * rTheNetwork;

};




#endif /* __FEEDFOWARDNEURALNETIMP_HH */
