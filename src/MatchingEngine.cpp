#include "../includes/MatchingEngine.hpp"
#include "../includes/OrderBook.hpp"

#include <iostream>

void MatchingEngine::printOrderBook() {

  std::cout << "Limit Buy Orders:\n";
  for (const auto &buyOrder : OrderBook::getOrderBook()->buyOrders) {
    std::cout << "Price: $" << buyOrder.first << "\n";
    for (const auto &order : buyOrder.second) {
      std::cout << "  Quantity: " << order.quantity << "\n";
    }
  }

  std::cout << "Market Buy Orders:\n";
  for (const auto &buyOrder : OrderBook::getOrderBook()->marketBuyOrders) {
    std::cout << "  Quantity: " << buyOrder.quantity << "\n";
  }

  std::cout << "Sell Orders:\n";
  for (const auto &sellOrder : OrderBook::getOrderBook()->sellOrders) {
    std::cout << "Price: $" << sellOrder.first << "\n";
    for (const auto &order : sellOrder.second) {
      std::cout << "  Quantity: " << order.quantity << "\n";
    }
  }

  std::cout << "Market Sell Orders:\n";
  for (const auto &sellOrder : OrderBook::getOrderBook()->marketSellOrders) {
    std::cout << "  Quantity: " << sellOrder.quantity << "\n";
  }

  std::cout << "Stop Orders:\n";
  for (const auto &stopOrder : OrderBook::getOrderBook()->stopOrders) {
    std::cout << "  Price: $" << stopOrder.stopPrice << "\n";
    std::cout << "  Quantity: " << stopOrder.quantity << "\n";
  }
}

void MatchingEngine::matchOrders() {
  // Step 1: Process Market Orders
  processMarketOrders();
  // Step 2: Process Stop Orders
  processStopOrders();
  // Step 3: Process Limit Orders
  processLimitOrders();
}

void MatchingEngine::processMarketOrders() {
  // Loop over market buy orders and match with the lowest sell orders
  while (!OrderBook::getOrderBook()->marketBuyOrders.empty() &&
         !OrderBook::getOrderBook()->sellOrders.empty()) {

    auto &marketBuyOrder = OrderBook::getOrderBook()->marketBuyOrders.front();
    auto &sellQueue = OrderBook::getOrderBook()->sellOrders.begin()->second;

    // Update the last traded price
    setLastTradedPrice(OrderBook::getOrderBook()->sellOrders.begin()->first);

    // Match the market buy order with the lowest sell orders
    matchMarketOrder(marketBuyOrder, sellQueue);

    // If after matching the market buy order quantity is 0, remove it
    if (marketBuyOrder.quantity == 0)
      OrderBook::getOrderBook()->marketBuyOrders.pop_front();
  }

  // Loop over market sell orders and match with the highest buy orders
  while (!OrderBook::getOrderBook()->marketSellOrders.empty() &&
         !OrderBook::getOrderBook()->buyOrders.empty()) {

    auto &marketSellOrder = OrderBook::getOrderBook()->marketSellOrders.front();
    auto &buyQueue = OrderBook::getOrderBook()->buyOrders.rbegin()->second;

    // Update the last traded price
    setLastTradedPrice(OrderBook::getOrderBook()->buyOrders.begin()->first);

    // Match the market sell order with the highest buy orders
    matchMarketOrder(marketSellOrder, buyQueue);

    // If after matching the market sell order quantity is 0, remove it
    if (marketSellOrder.quantity == 0)
      OrderBook::getOrderBook()->marketSellOrders.pop_front();
  }
}

void MatchingEngine::matchMarketOrder(MarketOrder &marketOrder,
                                      std::deque<LimitOrder> &orderQueue) {
  // Loop over the order queue and match the market order
  for (auto it = orderQueue.begin();
       it != orderQueue.end() && marketOrder.quantity > 0;) {

    // Get the matched quantity
    int matchQty = std::min(marketOrder.quantity, it->quantity);

    // Subtract the matched quantity from the market order and the limit order
    marketOrder.quantity -= matchQty;
    it->quantity -= matchQty;

    // Remove the limit order if it is fully matched then point to the next
    if (it->quantity == 0)
      it = orderQueue.erase(it);
    else
      ++it;
  }
}

void MatchingEngine::processStopOrders() {
  auto &stopOrders = OrderBook::getOrderBook()->stopOrders;
  // Flag to check if any stop orders have been triggered
  bool newMarketOrders = false;

  for (auto it = stopOrders.begin(); it != stopOrders.end();) {
    StopOrder &stopOrder = *it;

    bool trigger =
        (stopOrder.isBuy && stopOrder.stopPrice <= getLatestTradedPrice()) ||
        (!stopOrder.isBuy && stopOrder.stopPrice >= getLatestTradedPrice());

    if (trigger) {
      OrderBook::getOrderBook()->addOrder(new MarketOrder(
          stopOrder.orderID, stopOrder.traderID, stopOrder.isBuy,
          stopOrder.quantity, stopOrder.timestamp));
      it = stopOrders.erase(it);
      // Atleast one stop order has been triggered
      newMarketOrders = true;
    } else {
      ++it;
    }
  }

  // Process newly created market orders
  if (newMarketOrders) {
    processMarketOrders();
  }
}

void MatchingEngine::processLimitOrders() {
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

    // If a buy order has been found then update the last traded price
    setLastTradedPrice(sellPrice);

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

double MatchingEngine::getLatestTradedPrice() {
  // If the last traded price has not been set, calculate it using the midpoint
  // of the best bid and ask
  if (m_lastTradedPrice == 0.0) {
    if (OrderBook::getOrderBook()->buyOrders.empty() ||
        OrderBook::getOrderBook()->sellOrders.empty()) {
      return 0.0; // No market price available
    }

    double bestBid = OrderBook::getOrderBook()->buyOrders.rbegin()->first;
    double bestAsk = OrderBook::getOrderBook()->sellOrders.begin()->first;

    return (bestBid + bestAsk) / 2.0;
  } else {
    // Else just returnt he last traded price
    return m_lastTradedPrice;
  }
}
