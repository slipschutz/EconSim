
#ifndef __NETWORK_HH
#define __NETWORK_HH





#include "Eigen/Dense"
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::VectorXf;
using Eigen::Vector3d;
using Eigen::Vector2d;
using Eigen::Matrix;
using Eigen::Dynamic;

#include <vector>
#include <utility>

using namespace std;

class Network{
public:
  Network();
  Network(vector <int>);
  ~Network();


  void AddInputNode();
  void AddOutputNode();

  void SGD(vector <pair<VectorXd,VectorXd> > data,
	   int epochs, int mini_batch_size,double eta,int num=-1);

  void UpdateMiniBatch(vector <pair<VectorXd,VectorXd> > data,
		       vector <VectorXd>&,
		       vector <MatrixXd>&);




  VectorXd FeedFoward(vector<double>);
  VectorXd FeedFoward(VectorXd);


  void BackProp(vector <double> inputs,
		vector <double> trueAnswer,
		vector <VectorXd> & nb,
		vector <MatrixXd> &nw);

  void BackProp(VectorXd inputs,
		VectorXd trueAnswer,
		vector <VectorXd> & nb,
		vector <MatrixXd> &nw);


  void MakeEveryThing1();

  VectorXd cost_derivative(VectorXd output,
			   VectorXd y);

  vector<VectorXd> & GetBiases(){
    return biases2;
  }
  vector<MatrixXd> & GetWeights(){
    return weights2;
  }

  int GetNumberOfInputNodes(){
    return rSizes[0];
  }
  int GetNumberOfOutputNodes(){
    return rSizes[rSizes.size()-1];
  }



  
private:
  int rNumberLayers;
  vector <int> rSizes;

  vector <VectorXd> biases2;
  vector <MatrixXd> weights2;



  
};

template <typename T>
Matrix<T,Dynamic,1> Vector2Eigen(vector <T> in){

  Matrix<T,Dynamic,1> ret(in.size());
  for (int i=0;i<(int)in.size();i++){
    ret[i]=in[i];
  }
  return ret;

}


#endif /* __NETWORK_HH */
