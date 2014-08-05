#ifndef __RandomManager_HH
#define __RandomManager_HH

class RandomManager{
  
public:
  static int GetRand(int n);

private:
  RandomManager();
  ~RandomManager();
  static bool Seeded;

};


#endif
