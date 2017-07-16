
#ifndef NUMBERQUEUER_H_
#define NUMBERQUEUER_H_

#include <StandardCplusplus.h>
#include <vector>
#include <string>

#define r1 0
#define r2 8
#define r3 16
#define r4 24

using namespace std;

class NumberQueue 
{
public:
  NumberQueue(int rowNumber);
  bool hasInQueue();
  int popQueue();
  void push(int val);

  int rowNumber;

private:
  vector<int> queue;
};

#endif