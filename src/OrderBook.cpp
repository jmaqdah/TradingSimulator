#include "../includes/OrderBook.hpp"

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
    buyOrders[limitOrder.price].push_back(limitOrder);
  } else {
    sellOrders[limitOrder.price].push_back(limitOrder);
  }
}

void OrderBook::addMarketOrder(const MarketOrder &marketOrder) {
  if (marketOrder.isBuy)
    marketBuyOrders.push_back(marketOrder);
  else
    marketSellOrders.push_back(marketOrder);
}

void OrderBook::addStopOrder(const StopOrder &stopOrder) {
  stopOrders.push_back(stopOrder);
}