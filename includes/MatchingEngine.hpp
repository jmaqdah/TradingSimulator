#ifndef MATCHINGENGINE_HPP
#define MATCHINGENGINE_HPP

#include "LimitOrder.hpp"
#include "MarketOrder.hpp"
#include <deque>

class MatchingEngine {
public:
  MatchingEngine() = default;
  ~MatchingEngine() = default;

  void printOrderBook();
  void executeMatchingEngine();

private:
  void matchOrders();
  void processMarketOrders();
  void matchMarketOrder(MarketOrder &marketOrder,
                        std::deque<LimitOrder> &orderQueue);
  void processStopOrders();
  void processLimitOrders();
};

#endif // MATCHINGENGINE_HPP