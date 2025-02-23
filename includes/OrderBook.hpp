#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <deque>
#include <map>

#include "LimitOrder.hpp"

// I have made the OrderBook class a singleton class

class OrderBook {

public:
  static OrderBook *getOrderBook();

  std::map<double, std::deque<LimitOrder>> buyOrders;
  std::map<double, std::deque<LimitOrder>> sellOrders;

  void addOrder(LimitOrder order);

private:
  OrderBook();
  ~OrderBook();

  static OrderBook *instance;
};

#endif // ORDERBOOK_HPP