
#include "Queuer.h"

Queuer::Queuer(int rowNumber) {
  this->rowNumber = rowNumber;  
}

bool Queuer::hasInQueue() {
  return queue.size() > 0;
}

string Queuer::popQueue() {
  string a = queue.at(0);
  queue.erase(queue.begin());
  return a;
}

char Queuer::popQueueChar() {
  string a = queue.at(0);
  char b = a.at(0);
  a.erase(a.begin());
  queue.erase(queue.begin());
  if(a.size() > 0)
    queue.insert(queue.begin(), a);
  return b;
}

void Queuer::push(string val) {
  queue.push_back(val);
}
