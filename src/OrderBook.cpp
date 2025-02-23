#include "../includes/OrderBook.hpp"

// Define any static variables and functions
OrderBook *OrderBook::instance = nullptr;

OrderBook *OrderBook::getOrderBook() {

  if (OrderBook::instance == nullptr) {
    OrderBook::instance = new OrderBook();
  }

  return OrderBook::instance;
}

void OrderBook::addOrder(Order *order) {

  if (LimitOrder *limitOrder = dynamic_cast<LimitOrder *>(order)) {
    addLimitOrder(*limitOrder);
  } else if (MarketOrder *marketOrder = dynamic_cast<MarketOrder *>(order)) {
    addMarketOrder(*marketOrder);
  } else if (StopOrder *stopOrder = dynamic_cast<StopOrder *>(order)) {
    addStopOrder(*stopOrder);
  }
}

void OrderBook::addLimitOrder(const LimitOrder &limitOrder) {
  if (limitOrder.isBuy) {
    OrderBook::getOrderBook()->buyOrders[limitOrder.price].push_back(
        limitOrder);
  } else {
    OrderBook::getOrderBook()->sellOrders[limitOrder.price].push_back(
        limitOrder);
  }
}

void OrderBook::addMarketOrder(const MarketOrder &marketOrder) {
  if (marketOrder.isBuy)
    OrderBook::getOrderBook()->marketBuyOrders.push_back(marketOrder);
  else
    OrderBook::getOrderBook()->marketSellOrders.push_back(marketOrder);
}

void OrderBook::addStopOrder(const StopOrder &stopOrder) {
  OrderBook::getOrderBook()->stopOrders.push_back(stopOrder);
}

OrderBook::OrderBook() {}

OrderBook::~OrderBook() { delete OrderBook::instance; }