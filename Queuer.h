
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

class Queuer
{
public:
  Queuer(int rowNumber);
  bool hasInQueue();
  string popQueue();
  char popQueueChar();
  void push(string val);

  int rowNumber;

private:
  vector<string> queue;
};

#endif
