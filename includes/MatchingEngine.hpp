#ifndef MATCHINGENGINE_HPP
#define MATCHINGENGINE_HPP

#include "LimitOrder.hpp"
#include "MarketOrder.hpp"
#include "OrderBook.hpp"
#include <deque>

class MatchingEngine {
public:
  MatchingEngine(OrderBook *orderBook) : m_orderBook(orderBook) {};
  ~MatchingEngine();

  void printOrderBook();
  void addOrder(Order *order);

  void setLastTradedPrice(double lastTradedPrice);

  OrderBook *getOrderBook() { return m_orderBook; }

  double getLatestTradedPrice();

private:
  void processMarketOrders();
  void matchMarketOrder(MarketOrder &marketOrder,
                        std::deque<LimitOrder> &orderQueue);
  void processStopOrders(bool executeMarketOrders = true);
  void processLimitOrders();
  double m_lastTradedPrice = 0.0;
  OrderBook *m_orderBook;
};

#endif // MATCHINGENGINE_HPP