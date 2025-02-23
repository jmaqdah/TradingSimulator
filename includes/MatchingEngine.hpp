#ifndef MATCHINGENGINE_HPP
#define MATCHINGENGINE_HPP

#include "LimitOrder.hpp"

class MatchingEngine {
public:
  MatchingEngine() = default;
  ~MatchingEngine() = default;

  void printOrderBook();
  void executeMatchingEngine();

private:
  void matchOrders();
};

#endif // MATCHINGENGINE_HPP