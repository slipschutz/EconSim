
#include "MatrixMaker.hh"

void MatrixMaker::MakeMatrix (vector <Person*> p){
  
  vector <int> alreadyDone(p.size());
  
  // vector <vector <bool> > theMatrix;
  // theMatrix.resize(p.size());
  // for (int i=0;i<p.size();i++){
  //   theMatrix[i].resize(p.size());
  // }



  for (unsigned int i=0;i<p.size();i++){
    
    for (unsigned int j=0;j<p.size();j++){
      if ((p[i]->GetConnections())->count(j)==1){
	cout<<i<<" "<<j<<endl;
      }
    }
    
  }



}

