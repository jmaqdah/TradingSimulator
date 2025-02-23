#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <deque>
#include <map>

#include "LimitOrder.hpp"
#include "MarketOrder.hpp"
#include "StopOrder.hpp"

// I have made the OrderBook class a singleton class

class OrderBook {

public:
  static OrderBook *getOrderBook();

  std::map<double, std::deque<LimitOrder>> buyOrders;
  std::map<double, std::deque<LimitOrder>> sellOrders;
  std::deque<MarketOrder> marketBuyOrders;
  std::deque<MarketOrder> marketSellOrders;
  std::vector<StopOrder> stopOrders;

  void addOrder(Order *order);

private:
  OrderBook();
  ~OrderBook();
  void addLimitOrder(const LimitOrder &limitOrder);
  void addMarketOrder(const MarketOrder &marketOrder);
  void addStopOrder(const StopOrder &stopOrder);

  static OrderBook *instance;
};

#endif // ORDERBOOK_HPP