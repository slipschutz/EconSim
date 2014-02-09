#include "ErrorManager.hh"

void ErrorManager::BoxPrint(string text, string addOn){
  stringstream ss;
  ss<<text<<" "<<addOn;
  BoxPrint(ss.str());
}


void ErrorManager::BoxPrint(string text){

  cout<<endl;
  int maxWidth = 30;
  int length = text.length()+2;
  stringstream temp;

  if ( length <= maxWidth ){

    for (int i=0;i<length+4;i++)
      temp<<"*";

    cout<<right<<setw(length)<<temp.str();
    cout<<endl;
    cout<<"* "<<text<<"*"<<endl;
    cout<<right<<setw(length)<<temp.str();


  } else {
    for (int i=0;i<maxWidth+4;i++)
      temp<<"*";
    
    cout<<right<<setw(maxWidth+4)<<temp.str()<<endl;;
    int numLines = length /maxWidth;
    for (int i=0;i<numLines;i++){

      int spot=FindSpot(GetSpaces(text),maxWidth);
      int n=maxWidth+4-text.substr(0,spot).length()-1;
      cout<<"* "<<text.substr(0,spot)<<setw(n)<<right<<"*"<<endl;
      text=text.substr(spot+1,text.length());
    }
    int n=maxWidth+4-text.length()-1;
    cout<<"* "<<text<<setw(n)<<right<<"*"<<endl;
    cout<<right<<setw(maxWidth)<<temp.str()<<endl;;
  
 


  }


}

vector <int> ErrorManager::GetSpaces(string text){
  vector <int> r;
  for (int i=0;i<(int)text.length();i++){
    if (text[i] == ' ')
      r.push_back(i);
  }

  return r;
}

int ErrorManager::FindSpot(vector<int> spaces,int lineEnd){
  int min=100;
  int minSpot=-1;
  for (int i=0;i<(int)spaces.size();i++){
    if (lineEnd-spaces[i]<min && lineEnd-spaces[i]>0){
      min=lineEnd-spaces[i];
      minSpot=i;
    } else if (lineEnd-spaces[i]<0){
      break;
    }
  }

  return spaces[minSpot];
}

string ErrorManager::GetRepeat(char c,int n){
  stringstream s;
  for (int i=0;i<n;i++)
    s<<c;
  return s.str();
}
