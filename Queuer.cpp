
#include "Queuer.h"

Queuer::Queuer(int rowNumber) {
  this->rowNumber = rowNumber;  
}

bool Queuer::hasInQueue() {
  return queue.size() > 0;
}

String Queuer::popQueue() {
  String a = queue.at(0);
  queue.erase(queue.begin());
  return a;
}

char Queuer::popQueueChar() {
  String str = queue.at(0);
  char ch = str.charAt(0);
  str.remove(0,1);
  queue.erase(queue.begin());
  if(str.length() > 0) 
    queue.insert(queue.begin(), str); 
  return ch;
}

void Queuer::push(String val) {
  queue.push_back(val);
}
