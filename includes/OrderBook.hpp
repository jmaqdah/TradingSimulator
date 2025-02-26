#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <deque>
#include <map>

#include "LimitOrder.hpp"
#include "MarketOrder.hpp"
#include "StopOrder.hpp"

class OrderBook {

public:
  OrderBook() = default;
  ~OrderBook() = default;

  std::map<double, std::deque<LimitOrder>> buyOrders;
  std::map<double, std::deque<LimitOrder>> sellOrders;
  std::deque<MarketOrder> marketBuyOrders;
  std::deque<MarketOrder> marketSellOrders;
  std::vector<StopOrder> stopOrders;

  void addOrder(Order *order);

private:
  void addLimitOrder(const LimitOrder &limitOrder);
  void addMarketOrder(const MarketOrder &marketOrder);
  void addStopOrder(const StopOrder &stopOrder);

  static OrderBook *instance;
};

#endif // ORDERBOOK_HPP