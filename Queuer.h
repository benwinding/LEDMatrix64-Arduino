
#ifndef NUMBERQUEUER_H_
#define NUMBERQUEUER_H_

#include <Arduino.h>
#include <StandardCplusplus.h>
#include <vector>

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
  String popQueue();
  char popQueueChar();
  void push(String val);

  int rowNumber;

private:
  vector<String> queue;
};

#endif
