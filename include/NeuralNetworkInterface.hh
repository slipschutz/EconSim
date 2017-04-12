
#ifndef __NEURALNETWORKINTERFACE_HH
#define __NEURALNETWORKINTERFACE_HH


/**ABC to serve as interface for neural network implementation
   of economic actor decision making
 */
class NeuralNetworkInterface 
{
public:
  NeuralNetworkInterface();
  virtual ~NeuralNetworkInterface();
  
  virtual void AddInput()=0;
  virtual void AddAction()=0;
  virtual void Think()=0;
};



#endif /* __NEURALNETWORKINTERFACE_HH */
