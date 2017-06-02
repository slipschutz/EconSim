
#ifndef __NEURALNETWORKINTERFACE_HH
#define __NEURALNETWORKINTERFACE_HH

#include <string>
#include <functional>
#include <vector>

#include "Eigen/Dense"

/**ABC to serve as interface for neural network implementation
   of economic actor decision making
 */
class NeuralNetworkInterface 
{
public:
  NeuralNetworkInterface();
  virtual ~NeuralNetworkInterface();
  
  virtual void AddInput(std::string,double *)=0;
  virtual void AddAction(std::string,std::function<void()>)=0;
  virtual void SetInputReferences(std::vector<double> &)=0;
  virtual Eigen::VectorXd Think()=0;
  virtual void Print()=0;
  virtual void Train()=0;
  virtual NeuralNetworkInterface* MutateCopy(double x=10)=0;
  
};



#endif /* __NEURALNETWORKINTERFACE_HH */
