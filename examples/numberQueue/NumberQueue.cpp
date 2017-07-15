
#include "NumberQueue.h"

NumberQueue::NumberQueue(int rowNumber) {
  this->rowNumber = rowNumber;  
}

bool NumberQueue::hasInQueue() {
  return queue.size() > 0;
}

int NumberQueue::popQueue() {
  int a = queue.at(0);
  queue.erase(queue.begin());
  return a;
}

void NumberQueue::push(int val) {
  queue.push_back(val);
}