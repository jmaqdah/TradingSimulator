#include "../includes/MatchingEngine.hpp"
#include "../includes/OrderBook.hpp"

#include <iostream>

void MatchingEngine::printOrderBook() {
  std::cout << "Buy Orders:\n";
  for (const auto &buyOrder : OrderBook::getOrderBook()->buyOrders) {
    std::cout << "Price: $" << buyOrder.first << "\n";
    for (const auto &order : buyOrder.second) {
      std::cout << "  Quantity: " << order.quantity << "\n";
    }
  }

  std::cout << "Sell Orders:\n";
  for (const auto &sellOrder : OrderBook::getOrderBook()->sellOrders) {
    std::cout << "Price: $" << sellOrder.first << "\n";
    for (const auto &order : sellOrder.second) {
      std::cout << "  Quantity: " << order.quantity << "\n";
    }
  }
}

void MatchingEngine::matchOrders() {
  // Loop through all the sell orders
  for (auto it = OrderBook::getOrderBook()->sellOrders.begin();
       it != OrderBook::getOrderBook()->sellOrders.end();) {

    auto sellPrice = it->first;

    // If there are no buy orders at this sell price, move to the next sell
    // price
    if (OrderBook::getOrderBook()->buyOrders.find(sellPrice) ==
        OrderBook::getOrderBook()->buyOrders.end()) {
      ++it;
      continue;
    }

    auto &sellQueue = it->second;
    auto &buyQueue = OrderBook::getOrderBook()->buyOrders[sellPrice];

    // Calculate total available quantity on both sides
    int totalBuyQty = 0, totalSellQty = 0;
    for (const auto &order : buyQueue)
      totalBuyQty += order.quantity;
    for (const auto &order : sellQueue)
      totalSellQty += order.quantity;

    int matchedQty = std::min(totalBuyQty, totalSellQty);

    // Match buy orders orders via pro-rata allocation
    for (auto &order : buyQueue) {
      int qtyDecrement = std::round(static_cast<double>(order.quantity) *
                                    matchedQty / totalBuyQty);
      order.quantity -= qtyDecrement;
    }

    // Match sell orders orders via pro-rata allocation
    for (auto &order : sellQueue) {
      int qtyDecrement = std::round(static_cast<double>(order.quantity) *
                                    matchedQty / totalSellQty);
      order.quantity -= qtyDecrement;
    }

    // Remove fully matched orders
    buyQueue.erase(
        std::remove_if(buyQueue.begin(), buyQueue.end(),
                       [](const LimitOrder &o) { return o.quantity == 0; }),
        buyQueue.end());

    sellQueue.erase(
        std::remove_if(sellQueue.begin(), sellQueue.end(),
                       [](const LimitOrder &o) { return o.quantity == 0; }),
        sellQueue.end());

    // Remove empty price levels
    if (sellQueue.empty())
      it = OrderBook::getOrderBook()->sellOrders.erase(it);
    else
      ++it;

    if (buyQueue.empty())
      OrderBook::getOrderBook()->buyOrders.erase(sellPrice);
  }
}

void MatchingEngine::executeMatchingEngine() {
  std::cout << "Matching Engine Started!" << std::endl;
  matchOrders();
}
